/// Author: Przemysław Bągard

#ifndef OBJECTSHOWING_H
#define OBJECTSHOWING_H

#include "Object.h"

/// Object, which shows/hides another object, when character enters/exits this object.
class ObjectShowing : public Object
{
public:
    ObjectShowing(Object::Id who, bool hideAfter = false);

    /// Id of target object which will appear/dissapear.
    Object::Id who;

    /// Has to hide 'who' object after exiting this object?
    bool hideAfter;

    /// Shows object 'who'.
    virtual void enterAction(Object *);

    /// Hides object 'who', when 'hideAfter = true'.
    virtual void exitAction(Object *);
};

#endif // OBJECTSHOWING_H
