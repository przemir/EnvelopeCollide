/* 
 * File:   Plane.h
 * Author: Przemysław Bągard
 *
 * Created on 14 wrzesień 2010, 10:21
 */

#ifndef GEOMETRY_PLANE_H
#define	GEOMETRY_PLANE_H

#include "Point.h"

namespace EnvelopeGeometry {

/// plane
struct Plane {
    float A, B, C, D;

    Plane(): A(0), B(0), C(0), D(0) { }
    /// calculate the plane from 3 points
    Plane(const Point& p1, Point p2, Point p3);
    Plane(float iA, float iB, float iC, float iD): A(iA), B(iB), C(iC), D(iD) { }
    Plane(const Point& vec, float d = 0): A(vec.x), B(vec.y), C(vec.z), D(d) { }
};

/// returns a vector perpendicular to plane given as parameter
inline Point orthogonalToPlane(const Plane& p);

/// returns a plane perpendicular to plane given as parameter and contains points 'A' and 'B' (function assumes that A != B)
inline Plane orthogonalPlane(const Plane& p, const Point& A, const Point& B);

/// returns a plane containing point 'A' parallel to plane 'p'
inline Plane setParallelPlane(Plane p, const Point& A);

/// returns a value of the function p.A*A.x+p.B*A.y+p.C*A.z+p.D ; if value is positive then point is on the positive side of plane (consistent with normal vector)
inline float calculateSideOfPlane(const Plane& p, const Point& A);

/// returns distance from point 'A' to plane 'p'
inline float dist(const Plane& p, const Point& A);

/// returns D parameter value of plane described by orthogonal vector 'vec'. This plane contains point 'A'.
inline float calculatePlaneD(const Point& vec, const Point& A);

/// checks if at least one of the values of A,B,C is non-zero (including epsilon)
inline bool planeValid(const Plane& plane);

}

/******************************* implementacja *******************************/

namespace EnvelopeGeometry {

inline Plane::Plane(const Point& p1, Point p2, Point p3) {
    p2 -= p1;
    p3 -= p1;

    A = p2.y * p3.z - p3.y * p2.z;
    B = p2.z * p3.x - p3.z * p2.x;
    C = p2.x * p3.y - p3.x * p2.y;

    D = -(p1.x * A + p1.y * B + p1.z * C);
}

inline Point orthogonalToPlane(const Plane& p) {
    return Point(p.A, p.B, p.C);
}

inline Plane orthogonalPlane(const Plane& p, const Point& A, const Point& B) {
    return Plane(A, B, A + orthogonalToPlane(p));
}

inline Plane setParallelPlane(Plane p, const Point& A)
{
    p.D = -(A.x * p.A + A.y * p.B + A.z * p.C);
    return p;
}

inline float calculateSideOfPlane(const Plane& p, const Point& A)
{
    return p.A*A.x + p.B*A.y + p.C*A.z + p.D;
}
inline float dist(const Plane& p, const Point& A)
{
    return fabs(calculateSideOfPlane(p,A)) / dist(Punkt(p.A, p.B, p.C));
}

inline float calculatePlaneD(const Point& vec, const Point& A)
{
    return -(scalarProduct(vec, A));
}

inline bool planeValid(const Plane& plane)
{
    return !(isZero(plane.A) && isZero(plane.B) && isZero(plane.C));
}

}

#endif	/* GEOMETRY_PLANE_H */

