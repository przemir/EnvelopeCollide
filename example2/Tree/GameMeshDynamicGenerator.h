/// Author: Przemysław Bągard

#ifndef GAMEMESHDYNAMICGENERATOR_H
#define GAMEMESHDYNAMICGENERATOR_H

#include "Object.h"
#include "CollisionSystem.h"
#include "SolidGame.h"


/// Gets physical models of dynamic object from class T
struct GameDynamicMeshGenerator : EnvelopeCollide::System::MeshGenerator<SolidGame, Object::Id>
{
    /// Physical objects:
    /// - graphical model triangles - stored in object's graphical model (each graphical model's triangle is physical model here),
    /// - 'hard' shapes - stored in list in object,
    /// - 'soft' shapes - stored in list in object.

    /// Returns first physical model for T element.
    virtual bool first(SolidGame&, const SolidGame&);

    /// Returns next physical model for T element.
    /// Returns false, if there is no more physical objects, otherwise returns true and in 'data' variable store object information.
    virtual bool next(SolidGame&);

    /// If application logic criteria (not geometry) rejects this object, then whole category of physical models will be skipped.
    /// 'data' variable indicate to object's element, which will be considered as next (this element will also be checked by this function).
    virtual bool skip(SolidGame&);

    /// Generate object id (unique within a structure field).
    virtual Object::Id generateId(const SolidGame& source);

    /// Action to do before collision detection.
    /// This can be for example if we check collision for object with itself.
    virtual bool allow(const Object::Id& id_source, const Object::Id& id_current);
};

#endif // GAMEMESHDYNAMICGENERATOR_H
