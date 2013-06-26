/// Author: Przemysław Bągard

#ifndef RESULT_H
#define RESULT_H

#include "Const.h"
#include "Geometry/Point.h"

namespace EnvelopeCollide
{
    using namespace EnvelopeGeometry;

/// Generated data based on collision detection functions
template <typename T>
struct Result
{
    /// Collision event - enter/inside/exit object
    EnvelopeCollide::EVENT_MODE mode;

    /// Data of collided object
    T data;

    /// Vector perpendicular to surface
    Point norm;
};


}

#endif // RESULT_H
