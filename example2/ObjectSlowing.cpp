#include "ObjectSlowing.h"
#include "Player.h"

ObjectSlowing::ObjectSlowing()
{
}

void ObjectSlowing::stayAction(Object *obj)
{
    Object::stayAction(obj);
    Player* pl = dynamic_cast<Player*>(obj);
    pl->fullspeed = false;
}
