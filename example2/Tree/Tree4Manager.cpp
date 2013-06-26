/* 
 * File:   Tree4Manager.cpp
 * Author: Przemek
 * 
 * Created on 6 wrzesień 2011, 11:31
 */

#include "Tree4Manager.h"
#include "CollisionSystem/CollisionDetection.h"
#include "Engine.h"
#include<time.h>
#include "QPair"

extern Engine& en;

Tree4Manager::Tree4Manager()
{
}

void Tree4Manager::clearAll()
{
    treeS.clear();
    treeD.clear();
}

void Tree4Manager::prepare(int levels, const PKT& left_down, const PKT& right_up)
{
    rectRangeMin = Punkt(left_down.x, -120.0f, left_down.y);
    rectRangeMax = Punkt(right_up.x, 260.0f, right_up.y);
    treeS.init(levels, left_down, right_up);
    treeD.init(levels, left_down, right_up);
}

void Tree4Manager::addOnTilesetInitialize(Module *til)
{
    const QMap<Object::Id, Object *> &m = til->getAll();
    QMap<Object::Id, Object *>::const_iterator it;
    for(it = m.begin(); it != m.end(); ++it)
    {
        const Object* o = it.value();
        if(o->id != OBJECT_PLAYER)
            addOnTilesetInitialize(til->get(o->id));
    }
}

void Tree4Manager::addOnTilesetInitialize(Object* el)
{
    SolidGame atom;
    atom.id = el->id;

    if(!el->dynamic)
    {
        /// Static object.

        /// Is graphical model is also physical model?
        if(el->useModelAsHardShape)
        {
            atom.type = SolidGame::TYPE_GENERIC;
            for (int l1 = 0; l1 < el->model.getModelData().tri_n; ++l1) {
                atom.nr = l1;
                treeS.add(atom);
            }
        }

        /// 'hard' shapes.
        atom.type = SolidGame::TYPE_HARD;
        for (int l1 = 0; l1 < el->hardShape.size(); ++l1) {
            atom.nr = l1;
            treeS.add(atom);
        }

        /// 'soft' shapes.
        atom.type = SolidGame::TYPE_SOFT;
        for (int l1 = 0; l1 < el->softShape.size(); ++l1) {
            atom.nr = l1;
            treeS.add(atom);
        }
    }
    else
    {
        /// Dynamic object.

        if(el->atTree.second != 0)
            treeD.remove(el->atTree);
        if(el->exist)
            el->atTree = treeD.add(atom);
    }
}




void Tree4Manager::getInterfacesDetection(EnvelopeCollide::System::ResultCollector<SolidGame> &result, EnvelopeCollide::Entry &entry, EnvelopeCollide::System::Criteria<SolidGame> &criteria, Object::Id curr)
{
    treeS.getCollisionsDetection(result, entry, criteria, curr);
    treeD.getCollisionsDetection(result, entry, criteria, curr);
}

PKT Tree4Manager::getTreeRangeMin()
{
    return rectRangeMin;
}

PKT Tree4Manager::getTreeRangeMax()
{
    return rectRangeMax;
}

