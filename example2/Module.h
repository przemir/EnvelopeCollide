/// Author: Przemysław Bągard

#ifndef MODULE_H
#define MODULE_H

#include "Object.h"
#include "QMap"

/// Program world - position/properties of objects.
class Module
{
public:
    Module();

    /// Add object to module.
    bool add(Object* obj);

    /// Returns object with given id.
    Object* get(Object::Id id);

    /// Removes all objects from module.
    void clear();

    /// Initialize all objects in module.
    void initializeModule();

    /// Every frame routine.
    void processTime(float elapsed_time);

    /// Renders module objects.
    void render();

    /// Returns all module objects (read-only).
    const QMap<Object::Id, Object*>& getAll();
private:
    /// Module objects.
    QMap<Object::Id, Object*> _objects;
};

#endif // MODULE_H
