#include "Engine.h"
#include "Tree/SolidCriteriaGame.h"
#include "Tree/SolidGame.h"
#include "Tree/ResultCollectorAllQtTemplate.h"
#include "Tree/ResultCollectorCatcherQtTemplate.h"
#include <QDebug>

/** Global 'Engine' instance - store information within whole program. */
Engine& en = Engine::getInstance();

/**
  After writing:

extern Engine& en;

  in .cpp file, "en." will be equivalent to "Engine::get().".
**/

Engine::Engine()
{

}

float Engine::calculateRad(PKT norm)
{
    if(!arePointsNear(norm, Punkt(0,1,0)) && !arePointsNear(norm, Punkt(0,-1,0)))
        return getAngle(PKT0, Punkt(0,1,0), norm);
    else
        return 0.0f;
}

void Engine::processMove(float elapsed_time_with_direction)
{
    /// Calculates velocity
    float speed = 10.0;
    float speedMult = 1.0f;
    if(!pl->fullspeed)
    {
        speedMult = 0.25f;
        pl->fullspeed = true;
    }
    speed *= speedMult;

    elapsed_time_with_direction *= speed;

    /// Sets model's animation interpolation speed.
    if(pl->model.getCurrentState() == ANIMATION_STAND)
        pl->model.setAnimationSpeed(0.25f*speedMult);
    if(pl->model.getCurrentState() == ANIMATION_MOVE)
        pl->model.setAnimationSpeed(20.0f*speedMult);

    /// Calculates movement vector.
    PKT go = pl->front()*elapsed_time_with_direction;

    if(go != PKT0)
    {
        PKT go1, go2;
        PKT target_go = go;

        /// Detects 'hard' collisions on character's collision trajectory.
        EnvelopeCollide::Mesh data = pl->getCurrentHardShape(0);
        EnvelopeCollide::Mesh data1 = data;
        EnvelopeCollide::Entry entry;
        entry.makeFromAtom(data);
        entry.vec = go;
        SolidCriteriaGame<SolidGame> criteria;
        criteria.type = SolidCriteriaGame<SolidGame>::TYPE_COLLISION;
        ResultCollectorCatcherQtTemplate<SolidGame> result;

        /// Collsision detection routine between character and module objects.
        tree.getInterfacesDetection(result, entry, criteria, pl->id);
        PKT norm;
        bool coll = false;
        if(result.forwardb && result.forwardf <= 1.1f)  // zderzenie
        {
            qDebug() << "collide";

            float r = result.forwardf;
            coll = true;
            PKT vec = entry.vec*r;

            /// Character cannot enter 'hard' shape so movement vector is decreased slightly
            if(dist(vec) > 0.25f)
                go = vec - setLengthToVector(entry.vec, 0.1f);
            else go = PKT0;

            /// Vector perpendicular to 'hard' shape in moment of collision.
            norm = setLengthToVector(result.forward.norm);
            coll = true;
        }
        go1 = go;
        /// Movement of character.
        pl->pos += go;

        /// Points coordinates of character has changed.
        /// Shape data must be calculated again.
        data = pl->getCurrentHardShape(0);
        EnvelopeCollide::Mesh data2 = data;

        /// Wall slide.
        target_go -= go;
        PKT restGo = target_go;
        if(coll && !arePointsNear(target_go, PKT0))
        {
            float k = calculateRad(norm);

            PKT vvec1;
            PKT vvec2;
            PKT vvec = target_go;
            float MIN_KAT_NO_MOVABLE = 0.1;
            float MAX_KAT_NO_MOVABLE = 80;

            if(radToDeg(k) > MIN_KAT_NO_MOVABLE || radToDeg(k) < -MIN_KAT_NO_MOVABLE)
            {
                /// Large inclination angle - treat like wall.
                Projector proj(PKT0, setLengthToVector(norm), Punkt(0.0f, 1.0f, 0.0f));
                PKT a = proj.transform(vvec);
                PKT vv = proj.detransform(Punkt(0.0f, 0.0f, a.z));
                vvec2 = proj.detransform(Punkt(0.0f, a.y, 0.0f));
                vvec1 = vv;
                if(radToDeg(k) > MAX_KAT_NO_MOVABLE || radToDeg(k) < -MAX_KAT_NO_MOVABLE)
                    vvec2 = PKT0;
                vvec = vvec1 + vvec2;
            }
            else vvec.y = 0.0f;
            vvec.y = 0.0f;

            if(!arePointsNear(vvec, PKT0))
            {
                /// Slide vector not equals zero.
                /// Character moves according to slide vector for remaining velocity vector.
                PKT vv = vvec;
                result = ResultCollectorCatcherQtTemplate<SolidGame>();
                entry.vec = vv;
                tree.getInterfacesDetection(result, entry, criteria, pl->id);

                if(result.forwardb && result.forwardf <= 1.0f)  // zderzenie
                {
                    float r = result.forwardf;
                    PKT vec = entry.vec*r;
                    if(dist(vec) > 0.025f)
                        vv = vec - setLengthToVector(vec, 0.01f);
                    else vv = PKT0;
                }

                restGo -= vv;
                go2 = vv;
                pl->pos += vv;
            }
        }

        if(go1 != PKT0)
        {
            /// Character moves.
            /// Checks collisions with 'soft' shapes on movement vector and
            /// handles them by calling methods: 'stayAction', 'enterAction', 'exitAction'.

            EnvelopeCollide::Mesh data = data1;
            EnvelopeCollide::Entry entry;
            entry.makeFromAtom(data);
            entry.vec = go1;

            SolidCriteriaGame<SolidGame> criteria;
            criteria.type = SolidCriteriaGame<SolidGame>::TYPE_ACTION;
            ResultCollectorAllQtTemplate<SolidGame> result;
            tree.getInterfacesDetection(result, entry, criteria, pl->id);

            QMap<float, EnvelopeCollide::Result<SolidGame> >::iterator it;
            for(it = result.begin(); it != result.end(); ++it)
            {
                EnvelopeCollide::Result<SolidGame>& sr = it.value();
                if(sr.mode == EnvelopeCollide::EINSIDE)
                    module.get(sr.data.id)->stayAction(pl);
                if(sr.mode == EnvelopeCollide::EENTER)
                    module.get(sr.data.id)->enterAction(pl);
                if(sr.mode == EnvelopeCollide::EEXIT)
                    module.get(sr.data.id)->exitAction(pl);
            }
        }
        else
        {
            /// Character cannot move.
            /// Checks only if character is inside some 'soft' shape and handle it by calling method 'stayAction'.

            EnvelopeCollide::Mesh data = data1;
            EnvelopeCollide::Entry entry;
            entry.makeFromAtom(data);
            entry.vec = Point(1,0,0);

            SolidCriteriaGame<SolidGame> criteria;
            criteria.type = SolidCriteriaGame<SolidGame>::TYPE_ACTION;
            ResultCollectorAllQtTemplate<SolidGame> result;
            tree.getInterfacesDetection(result, entry, criteria, pl->id);

            QMap<float, EnvelopeCollide::Result<SolidGame> >::iterator it;
            for(it = result.begin(); it != result.end(); ++it)
            {
                EnvelopeCollide::Result<SolidGame>& sr = it.value();
                if(sr.mode == EnvelopeCollide::EINSIDE)
                    module.get(sr.data.id)->stayAction(pl);
            }
        }

        if(go2 != PKT0)
        {

            /// Character moves (slides).
            /// Checks collisions with 'soft' shapes on movement vector and
            /// handles them by calling methods: 'enterAction', 'exitAction'.
            /// Ignores events when character is inside some 'soft' shape.

            EnvelopeCollide::Mesh data = data2;
            EnvelopeCollide::Entry entry;
            entry.makeFromAtom(data);
            entry.vec = go2;

            SolidCriteriaGame<SolidGame> criteria;
            criteria.type = SolidCriteriaGame<SolidGame>::TYPE_ACTION;
            ResultCollectorAllQtTemplate<SolidGame> result;
            tree.getInterfacesDetection(result, entry, criteria, pl->id);

            QMap<float, EnvelopeCollide::Result<SolidGame> >::iterator it;
            for(it = result.begin(); it != result.end(); ++it)
            {
                EnvelopeCollide::Result<SolidGame>& sr = it.value();
                if(sr.mode == EnvelopeCollide::EENTER)
                    module.get(sr.data.id)->enterAction(pl);
                if(sr.mode == EnvelopeCollide::EEXIT)
                    module.get(sr.data.id)->exitAction(pl);
            }
        }
    }
    else
    {
        /// Character cannot slide.
        /// Checks only if character is inside some 'soft' shape and handle it by calling method 'stayAction'.

        EnvelopeCollide::Mesh data = pl->getCurrentHardShape(0);
        EnvelopeCollide::Entry entry;
        entry.makeFromAtom(data);
        entry.vec = Point(1,0,0);

        SolidCriteriaGame<SolidGame> criteria;
        criteria.type = SolidCriteriaGame<SolidGame>::TYPE_ACTION;
        ResultCollectorAllQtTemplate<SolidGame> result;
        tree.getInterfacesDetection(result, entry, criteria, pl->id);

        QMap<float, EnvelopeCollide::Result<SolidGame> >::iterator it;
        for(it = result.begin(); it != result.end(); ++it)
        {
            EnvelopeCollide::Result<SolidGame>& sr = it.value();
            if(sr.mode == EnvelopeCollide::EINSIDE)
                module.get(sr.data.id)->stayAction(pl);
        }
    }

    /// Sets camera behind character. Hovewer between character and camera cannot be obstacle.

    camera.mode = Camera::LOOKING_AT;
    camera.vAngle.x = (pl->radXZ+180);
    camera.vAngle.y = 10;
    camera.r = 9.0f;
    PKT O = pl->pos + Punkt(0, 3.2, 0);

    /// Check if camera collide with wall.
    EnvelopeCollide::Entry entry;
    entry.front_max = 1.2f;
    entry.mesh.p[0] = O + Punkt(0, 1, 0);
    entry.mesh.type = EnvelopeCollide::POINT;
    entry.vec = -pl->front()*camera.r - Punkt(0, 1, 0);
    entry.mesh.r = 0.01f;
    ResultCollectorCatcherQtTemplate<SolidGame> result;
    SolidCriteriaGame<SolidGame> criteria;
    criteria.type = SolidCriteriaGame<SolidGame>::TYPE_COLLISION;
    tree.getInterfacesDetection(result, entry, criteria, pl->id);

    if(result.forwardb)
    {
        /// Detects collision between camera and 'hard' shape.
        result.forwardf -= 0.2f;
        if(result.forwardf > 1.0f)
            result.forwardf = 1.0f;
        if(result.forwardf < 0.1f)
            result.forwardf = 0.1f;

        camera.r *= result.forwardf;
        O += Point(0, 1.0f-result.forwardf, 0);
    }
    camera.pos = O;
}
