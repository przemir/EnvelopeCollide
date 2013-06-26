#include "Module.h"

Module::Module()
{
}

bool Module::add(Object *obj)
{
    if(obj == NULL)
        return false;
    if(_objects.contains(obj->id))
        return false;

    _objects.insert(obj->id, obj);
    return true;
}

Object *Module::get(Object::Id id)
{
    if(_objects.contains(id))
        return _objects[id];
    else return NULL;
}

void Module::clear()
{
    _objects.clear();
}

void Module::initializeModule()
{
    Object* obj;
    foreach (obj, _objects) {
        obj->init();
    }
}

void Module::processTime(float elapsed_time)
{
    Object* obj;
    foreach (obj, _objects) {
        obj->processTime(elapsed_time);
    }
}


void Module::render()
{
    Object* obj;
    foreach (obj, _objects) {
        obj->render();
    }
}

const QMap<Object::Id, Object *> &Module::getAll()
{
    return _objects;
}
