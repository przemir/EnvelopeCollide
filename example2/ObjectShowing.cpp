#include "ObjectShowing.h"
#include "Engine.h"

extern Engine& en;

ObjectShowing::ObjectShowing(Id who, bool hideAfter) : Object(), who(who), hideAfter(hideAfter)
{
}

void ObjectShowing::enterAction(Object *)
{
    Object *el = en.module.get(who);
    if(!el->exist)
    {
        el->exist = true;
        en.tree.addOnTilesetInitialize(el);
    }
}

void ObjectShowing::exitAction(Object *)
{
    Object *el = en.module.get(who);
    if(el->exist && hideAfter)
    {
        el->exist = false;
        en.tree.addOnTilesetInitialize(el);
    }
}
