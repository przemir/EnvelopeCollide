#ifndef SEGMENT_H
#define SEGMENT_H

#include "Point.h"

namespace EnvelopeGeometry {


/// segment line with vertices a, b
struct Segment {
    Segment(const Point& a, const Point& b): a(a), b(b) {}

    Point a;
    Point b;
};

}

#endif // SEGMENT_H
