#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Point.h"

namespace EnvelopeGeometry {

/// triangle with vertices a, b, c
struct Triangle {
    Triangle(const Point& a, const Point& b, const Point& c): a(a), b(b), c(c) {}

    Point a;
    Point b;
    Point c;
};

}

#endif // TRIANGLE_H
