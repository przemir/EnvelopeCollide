/// Author: Przemysław Bągard

#ifndef MESH_H
#define MESH_H

#include "Const.h"
#include "Geometry/Point.h"
#include "assert.h"
#include "Geometry/Projector.h"
#include "Geometry/Point2D.h"
#include <iostream>

namespace EnvelopeCollide
{
    using namespace EnvelopeGeometry;

/// Envelope which contains three points or less
struct Mesh {
    /// Envelope's points
    Point p[3];
    /// Envelope's range
    float r;
    /// Envelope's skeleton shape - point/line segment/triangle
    EnvelopeCollide::SHAPE type;

    /// Envelope's skeleton shape vertex count
    int pCount() const;
};

}

/// implementation
namespace EnvelopeCollide
{

inline int Mesh::pCount() const
{
    switch(type)
    {
    case EnvelopeCollide::POINT: return 1; break;
    case EnvelopeCollide::LINE: return 2; break;
    case EnvelopeCollide::TRIANGLE: return 3; break;
    default:
        break;
    }
    return 0;
}

}

#endif // MESH_H
