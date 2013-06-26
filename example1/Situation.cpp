#include "Situation.h"

QTextStream &operator <<(QTextStream &stream, const Situation &myclass)
{
    return stream << myclass.gui << " " << myclass.pos.x << " " << myclass.pos.y << " " << myclass.pos.z << " " << myclass.rad << " " << myclass.camera.x << " " << myclass.camera.y << " " << myclass.camera.z << " " << myclass.cameraRad.x << " " << myclass.cameraRad.y << " " << myclass.frontVecLenght << " " << myclass.backVecLenght << " " << myclass.playerShape << " " << myclass.time << "\n";
}


QTextStream &operator >>(QTextStream &stream, Situation &myclass)
{
    return stream >> myclass.gui >> myclass.pos.x >> myclass.pos.y >> myclass.pos.z >> myclass.rad >> myclass.camera.x >> myclass.camera.y >> myclass.camera.z >> myclass.cameraRad.x >> myclass.cameraRad.y >> myclass.frontVecLenght >> myclass.backVecLenght >> myclass.playerShape >> myclass.time;
}
