/// Author: Przemysław Bągard

#ifndef RESULTCOLLECTORCATCHERQTTEMPLATE_H
#define RESULTCOLLECTORCATCHERQTTEMPLATE_H

#include "CollisionSystem/ResultCollector.h"
#include "CollisionSystem/Result.h"
#include "QMultiMap"

/// Gather only earliest collision when object moves forward and earliest collision when object goes back.
template<typename T>
class ResultCollectorCatcherQtTemplate : public EnvelopeCollide::System::ResultCollector<T>
{
public:
    ResultCollectorCatcherQtTemplate() { backb = false; forwardb = false; }

    /// Detected collision on front/back.
    bool backb, forwardb;

    /// Moment of collision on front/back.
    float backf, forwardf;

    /// Collided object data (back)
    EnvelopeCollide::Result<T> back;

    /// Collided object data (front)
    EnvelopeCollide::Result<T> forward;

    /// Operator called after collision detection procedure.
    virtual void operator() (const EnvelopeCollide::Entry&, EnvelopeCollide::System::Criteria<T>&, EnvelopeCollide::Functions::PierceRangeResult&, const T&);
};



/// implementation

template<typename T>
void ResultCollectorCatcherQtTemplate<T>::operator() (const EnvelopeCollide::Entry &entry, EnvelopeCollide::System::Criteria<T> &, EnvelopeCollide::Functions::PierceRangeResult &chosen, const T &atom)
{
    if(chosen.onCollisionTrajectory)
    {
        EnvelopeCollide::Result<T> detected;
        detected.data = atom;
        detected.mode = EnvelopeCollide::EENTER;
        detected.norm = chosen.vMinNorm();

        if(chosen.vMinDistance <= entry.front_max && chosen.vMinDistance > 0.0f)
        {
            /// Object enters another object moving forward.
            if(!forwardb || chosen.vMinDistance < forwardf)
            {
                forwardb = true;
                forwardf = chosen.vMinDistance;
                forward = detected;
            }
        }

        if(chosen.vMaxDistance <= 0.0f && chosen.vMaxDistance >= entry.back_max)
        {
            /// Object enters another object going back.
            if(!backb || chosen.vMaxDistance > backf)
            {
                backb = true;
                backf = chosen.vMaxDistance;
                back = detected;
            }
        }
    }
}

#endif // RESULTCOLLECTORCATCHERQTTEMPLATE_H
