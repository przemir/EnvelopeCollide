/// Author: Przemysław Bągard

#ifndef OBJECTSLOWING_H
#define OBJECTSLOWING_H

#include "Object.h"

/// Object, which slowns down character, as long as he stays in this object.
class ObjectSlowing : public Object
{
public:
    ObjectSlowing();

    /// Slowns down the character.
    virtual void stayAction(Object *obj);
};

#endif // OBJECTSLOWING_H
