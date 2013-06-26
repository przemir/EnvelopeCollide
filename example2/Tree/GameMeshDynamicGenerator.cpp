#include "GameMeshDynamicGenerator.h"
#include "CollisionSystem.h"
#include "Engine.h"

extern Engine& en;


bool GameDynamicMeshGenerator::first(SolidGame &data, const SolidGame& source)
{
    data = source;
    data.type = SolidGame::TYPE_GENERIC;
    data.nr = -1;
    return next(data);
}

bool GameDynamicMeshGenerator::next(SolidGame &data)
{
    Object* obj = en.module.get(data.id);
    ++data.nr;
    /// Next object belongs to the same category or go to the next category.
    if(data.type == SolidGame::TYPE_GENERIC && data.nr == obj->model.getModelData().tri_n)
    {
        data.type = SolidGame::TYPE_HARD;
        data.nr = 0;
    }
    if(data.type == SolidGame::TYPE_HARD && data.nr == obj->hardShape.size())
    {
        data.type = SolidGame::TYPE_SOFT;
        data.nr = 0;
    }
    if(data.type == SolidGame::TYPE_SOFT && data.nr == obj->softShape.size())
        return false; /// There is no more physical models related to this object.
    return true;
}

bool GameDynamicMeshGenerator::skip(SolidGame &data)
{
    /// Sets current index to the last physical model of this category ('next' will change category).
    Object* obj = en.module.get(data.id);
    if(data.type == SolidGame::TYPE_GENERIC)
    {
        data.nr = obj->model.getModelData().tri_n-1;
    }
    if(data.type == SolidGame::TYPE_HARD)
    {
        data.nr = obj->hardShape.size()-1;
    }
    if(data.type == SolidGame::TYPE_SOFT)
    {
        data.nr = obj->softShape.size()-1;
    }
    return next(data);
}

Object::Id GameDynamicMeshGenerator::generateId(const SolidGame &source)
{
    return source.id;
}

bool GameDynamicMeshGenerator::allow(const Object::Id &id_source, const Object::Id &id_current)
{
    return id_source != id_current;
}
