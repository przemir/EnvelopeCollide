/// Author: Przemysław Bągard

#ifndef GAMEMESHGENERATOR_H
#define GAMEMESHGENERATOR_H

#include "CollisionSystem/MeshGenerator.h"
#include "SolidGame.h"
#include "Object.h"

/// Gets physical models from class T
struct GameMeshGenerator : EnvelopeCollide::System::MeshGenerator<SolidGame, Object::Id>
{
    /// Returns first physical model for T element
    /// (always is first physical model here).
    inline virtual bool first(SolidGame&, const SolidGame&);

    /// Returns next physical model for T element.
    /// Returns false, if there is no more physical objects, otherwise returns true and in 'data' variable store object information
    /// ('data' always is related with one physical model here).
    inline virtual bool next(SolidGame&);

    /// If application logic criteria (not geometry) rejects this object, then whole category of physical models will be skipped.
    /// 'data' variable indicate to object's element, which will be considered as next (this element will also be checked by this function)
    /// ('data' always is related with one physical model here).
    inline virtual bool skip(SolidGame&);

    /// Generate object id (unique within a structure field).
    inline virtual Object::Id generateId(const SolidGame& source);

    /// Action to do before collision detection.
    /// This can be for example if we check collision for object with itself.
    inline virtual bool allow(const Object::Id& id_source, const Object::Id& id_current);
};

/// implementation

bool GameMeshGenerator::first(SolidGame&, const SolidGame&) { return true; }

bool GameMeshGenerator::next(SolidGame&) { return false; }

bool GameMeshGenerator::skip(SolidGame&) { return false; }

Object::Id GameMeshGenerator::generateId(const SolidGame &source) { return source.id; }

bool GameMeshGenerator::allow(const Object::Id &id_source, const Object::Id &id_current)
{ return id_source != id_current; }

#endif // GAMEMESHGENERATOR_H
