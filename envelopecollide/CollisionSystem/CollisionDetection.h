/// Author: Przemysław Bągard

#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "PierceRangeResult.h"
#include "Geometry/Plane.h"
#include "Geometry/Point.h"
#include "Geometry/math.h"
#include "Geometry/Projector.h"
#include "Geometry/Segment.h"
#include "Geometry/Triangle.h"

namespace EnvelopeCollide { namespace Functions {

/// For all of the following - v must not be (0,0,0) - collision detection for static objects can be done by using any non-zero vector
/// Radius should not equals zero (radius shoult be greater than epsilon)

/// Collision detection for displacemented point and point envelope (v != PKT0, r != 0)
PierceRangeResult pierceRangePointPoint(const PKT& A, const PKT& v, const PKT& D, float r);
/// Collision detection for displacemented point and line segment envelope (v != PKT0, r != 0)
PierceRangeResult pierceRangePointSegment(const PKT& s1, const PKT& v, const Segment& s2, float r);
/// Collision detection for displacemented point and triangle envelope (v != PKT0, r != 0)
PierceRangeResult pierceRangePointTriangle(const PKT& s1, const PKT& v, const Triangle& s2, float r);

/// Collision detection for displacemented line segment and point envelope (v != PKT0, r != 0)
PierceRangeResult pierceRangeSegmentPoint(const Segment& s1, const PKT& v, const PKT& s2, float r);
/// Collision detection for displacemented line segment and segment line envelope (v != PKT0, r != 0)
PierceRangeResult pierceRangeSegmentSegment(const Segment& s1, const PKT& v, const Segment& s2, float r);
/// Collision detection for displacemented line segment and triangle envelope (v != PKT0, r != 0)
PierceRangeResult pierceRangeSegmentTriangle(const Segment& s1, const PKT& v, const Triangle& s2, float r);

/// Collision detection for displacemented triangle and point envelope (v != PKT0, r != 0)
PierceRangeResult pierceRangeTrianglePoint(const Triangle& s1, const PKT& v, const PKT& s2, float r);
/// Collision detection for displacemented triangle and line segment envelope (v != PKT0, r != 0)
PierceRangeResult pierceRangeTriangleSegment(const Triangle& s1, const PKT& v, const Segment& s2, float r);
/// Collision detection for displacemented triangle and triangle envelope (v != PKT0, r != 0)
PierceRangeResult pierceRangeTriangleTriangle(const Triangle& s1, const PKT& v, const Triangle& s2, float r);

/// Collision detection for displacemented triangle and triangle envelope (deprecated! Function repeatedly performs the same code fragments) (v != PKT0, r != 0)
PierceRangeResult pierceRangeTriangleTriangleDeprecated(const Triangle& s1, const PKT& v, const Triangle& s2, float r);


}}

#endif // COLLISIONDETECTION_H
