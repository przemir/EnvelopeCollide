/* 
 * File:   Tree4Manager.h
 * Author: Przemek
 *
 * Created on 6 wrzesień 2011, 11:31
 */

#ifndef TREE4MANAGER_H
#define	TREE4MANAGER_H

#include "CollisionSystem.h"
#include "SolidGame.h"
#include "GameMeshExecutor.h"
#include "GameMeshGenerator.h"
#include "GameMeshDynamicGenerator.h"
#include "Object.h"
#include "Module.h"
#include "Geometry/Point.h"
#include "CollisionSystem/ProjectorMoveVectorFilter.h"
#include "CollisionSystem/BoundingSphereFilter.h"
#include "CollisionSystem/BoundingSphereSimpleFilter.h"
#include "CollisionSystem/ProjectorSimpleFilter.h"
#include "CollisionSystem/NoFilter.h"


/// Collision detection engine.
class Tree4Manager
{
public:
    Tree4Manager();

    /// Adds all module elements to octree.
    void addOnTilesetInitialize(Module* til);

    /// Adds object (together with its children) to octree.
    void addOnTilesetInitialize(Object* el);

    /// Clears all structures (trees).
    void clearAll();

    /// Initializes all structures holding object for collision detection and sets tree height.
    void prepare(int levels, const PKT& left_down, const PKT& right_up);

    /// Collision detection routine.
    void getInterfacesDetection(EnvelopeCollide::System::ResultCollector<SolidGame> &result, EnvelopeCollide::Entry& entry, EnvelopeCollide::System::Criteria<SolidGame> &criteria, Object::Id el);

    /// Returns left, down, bottom point.
    PKT getTreeRangeMin();

    /// Returns right, up, top point.
    PKT getTreeRangeMax();

private:
    /// Static tree (all object coordinates are constant after loading module).
    EnvelopeCollide::System::OctreeStructure<SolidGame, GameMeshExecutor<SolidGame>, GameMeshGenerator, Object::Id, EnvelopeCollide::System::ProjectorMoveVectorFilter, EnvelopeCollide::System::ReadOnlyFieldContainer> treeS;

    /// dynamic treee (object changes their position or appears/disappears).
    EnvelopeCollide::System::OctreeStructure<SolidGame, GameMeshExecutor<SolidGame>, GameDynamicMeshGenerator, Object::Id, EnvelopeCollide::System::ProjectorMoveVectorFilter, EnvelopeCollide::System::WriteFieldContainer> treeD;

    /// Left, down, bottom point.
    PKT rectRangeMin;

    /// Right, up, top point.
    PKT rectRangeMax;
};


#endif	/* TREE4MANAGER_H */

