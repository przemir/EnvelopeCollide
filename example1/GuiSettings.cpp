#include "GuiSettings.h"

QTextStream &operator <<(QTextStream &stream, const GuiSettings &myclass)
{
    return stream << myclass.showObjectSolid << " " << myclass.showPlayerSolid << " " << myclass.showMoveVector << " " << myclass.showPlane << " " << myclass.logMode << " " << myclass.criteriaMode;
}


QTextStream &operator >>(QTextStream &stream, GuiSettings &myclass)
{
    int tempVec, tempPlane;
    stream >> myclass.showObjectSolid >> myclass.showPlayerSolid >> tempVec >> tempPlane >> myclass.logMode >> myclass.criteriaMode;
    myclass.showMoveVector = tempVec;
    myclass.showPlane = tempPlane;
    return stream;
}
