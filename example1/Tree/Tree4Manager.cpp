/* 
 * File:   Tree4Manager.cpp
 * Author: Przemek
 * 
 * Created on 6 wrzesień 2011, 11:31
 */

#include "Tree4Manager.h"
#include "QPair"

Tree4Manager::Tree4Manager()
{
}

void Tree4Manager::clearAll()
{
    treeS.clear();
}

void Tree4Manager::prepare(int levels, const PKT& left_down, const PKT& right_up)
{
    rectRangeMin = Punkt(left_down.x, -120.0f, left_down.y);
    rectRangeMax = Punkt(right_up.x, 260.0f, right_up.y);
    treeS.init(levels, left_down, right_up);
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
    atom.nr = 0;

    /// Is graphical model is also physical model?
    if(el->useModelAsShape)
    {
        atom.type = SolidGame::TYPE_GENERIC;
        for (int l1 = 0; l1 < el->model.getModelData().tri_n; ++l1) {
            atom.nr = l1;
            treeS.add(atom);
        }
    }

    if(el->shapeType == Object::SHAPE_NORMAL)
    {
        /// Adds normal shape
        atom.type = SolidGame::TYPE_NORMAL;
        /// Adds object to octree (if this object has any physical model)
        treeS.add(atom);
    }
    else
    {
        /// Adds distinguished shape
        atom.type = SolidGame::TYPE_SPECIAL;
        /// Adds object to octree (if this object has any physical model)
        treeS.add(atom);
    }
}

void Tree4Manager::getInterfacesDetection(EnvelopeCollide::System::ResultCollector<SolidGame> &result, EnvelopeCollide::Entry &entry, EnvelopeCollide::System::Criteria<SolidGame> &criteria, Object::Id curr)
{
    treeS.getCollisionsDetection(result, entry, criteria, curr);
}

PKT Tree4Manager::getTreeRangeMin()
{
    return rectRangeMin;
}

PKT Tree4Manager::getTreeRangeMax()
{
    return rectRangeMax;
}

