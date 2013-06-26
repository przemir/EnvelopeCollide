/*
 * File:   Point2D.h
 * Author: Przemysław Bągard
 * Created on 14 wrzesień 2010, 10:21
 */

#ifndef GEOMETRY_POINT2D_H
#define	GEOMETRY_POINT2D_H

#include <cmath>
#include "const.h"
#include "math.h"

namespace EnvelopeGeometry {

/// 2D point structure
template <typename T>
struct Point2D {
    T x, y;

    Point2D<T>(): x(0), y(0) { }
    Point2D<T>(T x, T y): x(x), y(y) { }

    static const Point2D<T> Zero;
};

typedef Point2D<float> fPoint;
typedef fPoint fPKT;
typedef fPoint fPunkt;
typedef Point2D<int> iPoint;
typedef iPoint iPKT;
typedef iPoint iPunkt;

#define fPointZero fPoint::Zero
#define fPKT0 fPoint::Zero
#define iPointZero iPoint::Zero
#define iPKT0 iPoint::Zero

// Point2D operators

template <typename T>
inline bool operator ==(const Point2D<T>& a, const Point2D<T> & b);
template <typename T>
inline bool operator !=(const Point2D<T>& a, const Point2D<T> & b);
template <typename T>
inline bool epsilonEqual(const Point2D<T>& a, const Point2D<T> & b);
template <typename T>
inline Point2D<T> operator -(const Point2D<T>& p);
template <typename T>
inline Point2D<T> operator +(const Point2D<T>& a, const Point2D<T>& b);
template <typename T>
inline Point2D<T> operator -(const Point2D<T>& a, const Point2D<T>& b);
template <typename T>
inline Point2D<T> operator *(const Point2D<T>& a, const T& b);
template <typename T>
inline Point2D<T> operator /(const Point2D<T>& a, const T& b);
template <typename T>
inline Point2D<T>& operator +=(Point2D<T>& a, const Point2D<T>& b);
template <typename T>
inline Point2D<T>& operator -=(Point2D<T>& a, const Point2D<T>& b);
template <typename T>
inline Point2D<T>& operator *=(Point2D<T>& a, const T& b);
template <typename T>
inline Point2D<T>& operator /=(Point2D<T>& a, const T& b);

/// lexicographic comparision 2D points as pairs (x,y)
template <typename T>
inline bool operator <(const Point2D<T>& a, const Point2D<T>& b);

/// checks if vector 'p1' is parallel to vector 'p2'
template <typename T>
inline bool parallelVector(const Point2D<T>& p1, const Point2D<T>& p2);

/// returns determinant of matrix with rows: 'p1', 'p2'
template <typename T>
inline T det(const Point2D<T>& p1, const Point2D<T>& p2);

/// returns square of the distance (default coordinates of 'p2' are (0,0))
template <typename T>
inline T dists(Point2D<T> p1, const Point2D<T>& p2 = Point2D<T>::Zero);

/// returns distance (default coordinates of 'p2' are (0,0))
template <typename T>
inline float dist(const Point2D<T>& p1, const Point2D<T>& p2 = Point2D<T>::Zero);

/// returns distance in metric described by function 'abs(p2.x-p1.x)+abs(p2.y-p1.y)+abs(p2.z-p1.z)' (default coordinates of 'p2' are (0,0))
template <typename T>
inline T distm(const Point2D<T>& p1, const Point2D<T>& p2 = Point2D<T>::Zero);

/// returns scalar product on vectors 'p1', 'p2'
template <typename T>
inline T scalarProduct(const Point2D<T>& p1, const Point2D<T>& p2);

/// checks if point 'p' is in clockwise direction to line segment 'ab'
template <typename T>
inline bool pointOnRight(const Point2D<T>& a, const Point2D<T>& b, const Point2D<T>& p);

/// checks if point 'p' is in counterclockwise direction to line segment 'ab'
template <typename T>
inline bool pointOnLeft(const Point2D<T>& a, const Point2D<T>& b, const Point2D<T>& p);

/// checks if point 'p' is on line segment 'ab'
template <typename T>
inline bool pointOnLine(const Point2D<T>& a, const Point2D<T>& b, const Point2D<T>& p);

/// checks if vector 'p1' is in clockwise direction to vector 'p2'
template <typename T>
inline bool vectorOnRight(const Point2D<T>& p1, const Point2D<T>& p2);

/// checks if vector 'p1' is in counterclockwise direction to vector 'p2'
template <typename T>
inline bool vectorOnLeft(const Point2D<T>& p1, const Point2D<T>& p2);

/// checks if point 'p' is inside cuboid described by two contrary points 'a' and 'a+v'
template <typename T>
inline bool inRectSize(const Point2D<T>& a, const Point2D<T>& v, const Point2D<T>& p);

/// checks if point 'p' is inside triangle 'abc' ?
template <typename T>
inline bool inTriangle(const Point2D<T>& a, const Point2D<T>& b, const Point2D<T>& c, const Point2D<T>& p);

/// returns rotated 2D point with parameters: 'k' - rotation angle (in radians), 'o' - center of rotation, 'a' - point coordinates before rotation
template <typename T>
inline Point2D<T> rotate(const Point2D<T>& o, Point2D<T> a, const float& k);

/// returns rotated 2D point with parameters: 'k' - rotation angle (in radians), '(0,0)' - center of rotation, 'a' - point coordinates before rotation
template <typename T>
inline Point2D<T> rotate(const Point2D<T>& a, const float& k);

/// returns angle between vectors 'OA', 'OB' (in the range '(-pi, pi]' )
template <typename T>
inline float getAngle(const Point2D<T>& O, const Point2D<T>& A, const Point2D<T>& B);

/// returns vector parallel to vector 'a' with length 'r'
inline fPoint setLengthToVector(const fPoint& a, const float& r = 1.0f);

/// returns scalar product on vectors 'p1', 'p2' (returned value has type double)
inline double scalarProductDouble(const fPKT& p1, const fPKT& p2);

/// conversion: degrees to radians
inline fPKT degToRad(const fPKT& k);

/// conversion: radians to degrees
inline fPKT radToDeg(const fPKT& k);

/// returns vector perpendicular to vectors 'vec'
inline fPKT orthogonalVector(fPKT vec);

}

/******************************* implementation *******************************/

namespace EnvelopeGeometry {

template <typename T>
const Point2D<T> Point2D<T>::Zero = Point2D<T>();

template <typename T>
inline Point2D<T> operator -(const Point2D<T>& p) {
    return Point2D<T>(-p.x, -p.y);
}

template <typename T>
inline Point2D<T> operator +(const Point2D<T>& a, const Point2D<T>& b) {
    return Point2D<T>(a.x + b.x, a.y + b.y);
}

template <typename T>
inline Point2D<T> operator -(const Point2D<T>& a, const Point2D<T>& b) {
    return Point2D<T>(a.x - b.x, a.y - b.y);
}

template <typename T>
inline Point2D<T> operator *(const Point2D<T>& a, const T& b) {
    return Point2D<T>(a.x * b, a.y * b);
}

template <typename T>
inline Point2D<T> operator /(const Point2D<T>& a, const T& b) {
    return Point2D<T>(a.x / b, a.y / b);
}

template <typename T>
inline bool operator ==(const Point2D<T>& a, const Point2D<T>& b) {
    return (a.x == b.x && a.y == b.y);
}

template <typename T>
inline bool operator !=(const Point2D<T>& a, const Point2D<T>& b) {
    return !(a == b);
}

template <typename T>
inline Point2D<T>& operator +=(Point2D<T>& a, const Point2D<T>& b) {
    a.x += b.x;
    a.y += b.y;
    return a;
}

template <typename T>
inline Point2D<T>& operator -=(Point2D<T>& a, const Point2D<T>& b) {
    a.x -= b.x;
    a.y -= b.y;
    return a;
}

template <typename T>
inline Point2D<T>& operator *=(Point2D<T>& a, const T& b) {
    a.x *= b;
    a.y *= b;
    return a;
}

template <typename T>
inline Point2D<T>& operator /=(Point2D<T>& a, const T& b) {
    a.x /= b;
    a.y /= b;
    return a;
}

template <typename T>
inline bool operator <(const Point2D<T>& a, const Point2D<T>& b) {
    return a.x < b.x || (a.x == b.x && a.y < b.y);
}

template <typename T>
inline bool epsilonEqual(const Point2D<T>& a, const Point2D<T>& b) {
    return isZero(dist(a, b));
}

template <typename T>
inline T det(const Point2D<T>& p1, const Point2D<T>& p2) {
    return p1.x * p2.y - p2.x * p1.y;
}

template <typename T>
inline T dists(Point2D<T> p1, const Point2D<T>& p2 = Point2D<T>::Zero) {
    p1 -= p2;
    return p1.x * p1.x + p1.y * p1.y;
}

template <typename T>
inline float dist(const Point2D<T>& p1, const Point2D<T>& p2 = Point2D<T>::Zero) {
    return sqrt(dists(p1, p2));
}

template <typename T>
inline T distm(const Point2D<T>& p1, const Point2D<T>& p2 = Point2D<T>::Zero) {
    return abs(p2.x - p1.x) + abs(p2.y - p1.y);
}

template <typename T>
inline T scalarProduct(const Point2D<T>& p1, const Point2D<T>& p2) {
    return p1.x * p2.x + p1.y * p2.y;
}

template <typename T>
inline bool pointOnRight(const Point2D<T>& a, const Point2D<T>& b, const Point2D<T>& p) {
    return isNegative(det(b - a, p - a));
}

template <typename T>
inline bool pointOnLeft(const Point2D<T>& a, const Point2D<T>& b, const Point2D<T>& p) {
    return isPositive(det(b - a, p - a));
}

template <typename T>
inline bool pointOnLine(const Point2D<T>& a, const Point2D<T>& b, const Point2D<T>& p) {
    return isZero(det(b - a, p - a));
}

template <typename T>
inline bool vectorOnRight(const Point2D<T>& p1, const Point2D<T>& p2) {
    return isPositive(det(p1, p2));
}

template <typename T>
inline bool vectorOnLeft(const Point2D<T>& p1, const Point2D<T>& p2) {
    return isNegative(det(p1, p2));
}

template <typename T>
inline bool parallelVector(const Point2D<T>& p1, const Point2D<T>& p2) {
    return isZero(det(p1, p2));
}

template <typename T>
inline bool inRectSize(const Point2D<T>& a, const Point2D<T>& v, const Point2D<T>& p) {
    return (p.x >= a.x && p.x <= a.x + v.x && p.y >= a.y && p.y <= a.y + v.y);
}

template <typename T>
inline bool inTriangle(const Point2D<T>& a, const Point2D<T>& b, const Point2D<T>& c, const Point2D<T>& p) {
    if (pointOnRight(a, b, c)) {
        // zgodnie z wskazówkami zegara
        return pointOnRight(a, b, p) && pointOnRight(b, c, p) && pointOnRight(c, a, p);
    }

    // przeciwnie do wskazówek zegara
    return pointOnRight(a, c, p) && pointOnRight(c, b, p) && pointOnRight(b, a, p);
}

template <typename T>
inline Point2D<T> rotate(const Point2D<T>& o, Point2D<T> a, const float& k) {
    if (k == 0)
        return a;

    a -= o;
    a = rotate(a, k);
    a += o;
    return a;
}

template <typename T>
inline Point2D<T> rotate(const Point2D<T>& a, const float& k) {
    if (k == 0)
        return a;

    float sink = sinf(k);
    float cosk = cosf(k);

    return Point2D<T>(a.x * cosk - a.y * sink, a.x * sink + a.y * cosk);
}

template <typename T>
inline float getAngle(const Point2D<T>& O, const Point2D<T>& A, const Point2D<T>& B) {
    float a2 = dists(O, A);
    float b2 = dists(O, B);
    float c2 = dists(A, B);

    if (b2 == 0 || a2 == 0)
        return 0.0f;

    float cos_alfa = (a2 + b2 - c2) / (2 * sqrt(a2 * b2));
    setInRange(-1.0f, cos_alfa, 1.0f);

    float rad = acosf(cos_alfa);

    if (det(A - O, B - O) < 0)
        return -rad;
    else
        return rad;
}

inline fPoint setLengthToVector(const fPoint& a, const float& r) {
    return a*(float)(sqrt((r*r)/dists(a)));
}

inline double scalarProductDouble(const fPKT& p1, const fPKT& p2) {
    return (double)p1.x * (double)p2.x + (double)p1.y * (double)p2.y;
}

inline fPKT degToRad(const fPKT& k) { return fPunkt((k.x*M_PI)/180.0, (k.y*M_PI)/180.0); }
inline fPKT radToDeg(const fPKT& k) { return fPunkt((k.x/M_PI)*180.0, (k.y/M_PI)*180.0); }

inline fPKT orthogonalVector(fPKT vec)
{
    return fPunkt(vec.y, -vec.x);
}

}

#endif

