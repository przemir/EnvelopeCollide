/* 
 * File:   Point.h
 * Author: Przemysław Bągard
 *
 * Created on 14 wrzesień 2010, 10:21
 */

#ifndef GEOMETRY_POINT_H
#define	GEOMETRY_POINT_H

//#include <QDebug>

#include <cmath>
#include "math.h"
#include "const.h"
#include "Point2D.h"

namespace EnvelopeGeometry {

/// 3D point or vector structure
struct Point {
    float x, y, z;

    Point(): x(0.0f), y(0.0f), z(0.0f) { }
    Point(float ix, float iy, float iz): x(ix), y(iy), z(iz) { }

    static const Point Zero;
};

//const Point Point::Zero = Point();

typedef Point Punkt;
typedef Point PKT;

#define PointZero Point::Zero
#define PKT0 Point::Zero

/// 3D (integer coordinates) point or vector structure
struct iPoint3D
{
    int x, y, z;
    iPoint3D(): x(0), y(0), z(0) { }
    iPoint3D(float ix, float iy, float iz): x(ix), y(iy), z(iz) { }

    static const iPoint3D Zero;
};

// Point: operators

bool operator==(const Point& a, const Point& b);
bool operator!=(const Point& a, const Point& b);
Point operator-(const Point& p);
Point operator+(const Point& a, const Point& b);
Point operator-(const Point& a, const Point& b);
Point operator*(const Point& a, const float& b);
Point operator/(const Point& a, const float& b);
Point& operator+=(Point& a, const Point& b);
Point& operator-=(Point& a, const Point& b);
Point& operator*=(Point& a, const float& b);
Point& operator/=(Point& a, const float& b);

/// returns determinant of matrix with rows: 'p1', 'p2', 'p3'
float det(const Point& p1, const Point& p2, const Point& p3);

/// returns square of the distance (default coordinates of 'p2' are (0,0,0))
float dists(Point p1, const Point p2);

/// returns distance (default coordinates of 'p2' are (0,0,0))
float dist(Point p1, const Point p2);

/// returns distance in metric described by function 'abs(p2.x-p1.x)+abs(p2.y-p1.y)+abs(p2.z-p1.z)' (default coordinates of 'p2' are (0,0,0))
float distm(Point p1, const Point p2);

/// returns scalar product on vectors 'p1', 'p2'
float scalarProduct(const Point& p1, const Point& p2);

/// checks if point 'p' is inside rectangle described by two contrary points 'a' and 'a+v'
bool inRectSize(const Point& a, const Point& v, const Point& p);

/// checks if point 'p' is on line 'ab'
bool pointOnLine(const Point& a, const Point& b, const Point& p);

/* rotations */
/// returns 3D point rotated in the plane XZ with parameters: 'k' - rotation angle (in radians), 'o' - center of rotation, 'a' - point coordinates before rotation
Point rotateZX(const Point& o, Point a, const float& k);

/// returns 3D point rotated in the plane XZ with parameters: 'k' - rotation angle (in radians), '(0,0,0)' - center of rotation, 'a' - point coordinates before rotation
Point rotateZXAround0(const Point& a, const float& k);

/// returns 3D point rotated in the plane XY with parameters: 'k' - rotation angle (in radians), 'o' - center of rotation, 'a' - point coordinates before rotation
Point rotateXY(const Point& o, Point a, const float& k);

/// returns 3D point rotated in the plane XY with parameters: 'k' - rotation angle (in radians), '(0,0,0)' - center of rotation, 'a' - point coordinates before rotation
Point rotateXYAround0(const Point& a, const float& k);

/// returns 3D point rotated in the plane YZ with parameters: 'k' - rotation angle (in radians), 'o' - center of rotation, 'a' - point coordinates before rotation
Point rotateYZ(const Point& o, Point a, const float& k);

/// returns 3D point rotated in the plane YZ with parameters: 'k' - rotation angle (in radians), '(0,0,0)' - center of rotation, 'a' - point coordinates before rotation
Point rotateYZAround0(const Point& a, const float& k);

/// returns point with applied spherical coordinates 'k1', 'k2' (in radians) with 'r' - sphere range, 'o' - center of sphere
Point setSphericalRotation(const Point& o, const float& r, const float& k1, const float& k2);

/// returns spherical coordinate 'k2' (in radians)
double getSphericalK2(const Point& o, const Point& p);

/// returns spherical coordinate 'k1' (rotation in the plane XZ) (in radians)
double getSphericalK1(const Point& o, const Point& p);

/// returns angle between vectors 'OA', 'OB' (in the range '[0, pi)' )
float getAngle(const Point& O, const Point& A, const Point& B);

/// returns vector parallel to vector 'a' with length 'r'
Point setLengthToVector(const Point& a, const float& r = 1.0f);

/// checks if points 'p1', 'p2' are in dinstance lesser than epsilon
bool arePointsNear(const Point& p1, const Point& p2);

/// checks if line segments 'AB' and 'CD' are parallel
bool linesParallel(const Point& A, const Point& B, const Point& C, const Point& D);

/// returns fPoint with coordinates (x,y)
inline fPKT pointWithoutZ(const PKT& pkt);

/// returns vector perpendicular to vectors 'b1', 'b2'
inline PKT orthoToVec(const PKT& b1, const PKT& b2);


/// iPoint3D: operators

bool operator==(const iPoint3D& a, const iPoint3D& b);
bool operator!=(const iPoint3D& a, const iPoint3D& b);
iPoint3D operator-(const iPoint3D& p);
iPoint3D operator+(const iPoint3D& a, const iPoint3D& b);
iPoint3D operator-(const iPoint3D& a, const iPoint3D& b);
iPoint3D operator*(const iPoint3D& a, const int& b);
iPoint3D operator/(const iPoint3D& a, const int& b);
iPoint3D& operator+=(iPoint3D& a, const iPoint3D& b);
iPoint3D& operator-=(iPoint3D& a, const iPoint3D& b);
iPoint3D& operator*=(iPoint3D& a, const int& b);
iPoint3D& operator/=(iPoint3D& a, const int& b);

}

/******************************* implementation *******************************/

namespace EnvelopeGeometry {

inline Point operator-(const Point& p) {
    return Point(-p.x, -p.y, -p.z);
}

inline Point operator+(const Point& a, const Point& b) {
    return Point(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Point operator-(const Point& a, const Point& b) {
    return Point(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline Point operator*(const Point& a, const float& b) {
    return Point(a.x * b, a.y * b, a.z * b);
}

inline Point operator/(const Point& a, const float& b) {
    return Point(a.x / b, a.y / b, a.z / b);
}

inline bool operator==(const Point& a, const Point& b) {
    return (a.x == b.x && a.y == b.y && a.z == b.z);
}

inline bool operator!=(const Point& a, const Point& b) {
    return !(a == b);
}

inline Point & operator+=(Point& a, const Point& b) {
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    return a;
}

inline Point & operator-=(Point& a, const Point& b) {
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
    return a;
}

inline Point & operator*=(Point& a, const float& b) {
    a.x *= b;
    a.y *= b;
    a.z *= b;
    return a;
}

inline Point & operator/=(Point& a, const float& b) {
    a.x /= b;
    a.y /= b;
    a.z /= b;
    return a;
}

inline float det(const Point& p1, const Point& p2, const Point& p3) {
    return p1.x * p2.y * p3.z + p1.y * p2.z * p3.x + p1.z * p2.x * p3.y - p3.x * p2.y * p1.z - p3.y * p2.z * p1.x - p3.z * p2.x * p1.y;
}

inline float dists(Point p1, const Point p2 = PKT0) {
    p1 -= p2;
    return p1.x * p1.x + p1.y * p1.y + p1.z * p1.z;
}

inline float dist(Point p1, const Point p2 = PKT0) {
    return sqrt(dists(p1, p2));
}

inline float distm(Point p1, const Point p2 = PKT0) {
    return fabs(p2.x - p1.x) +fabs(p2.y - p1.y) + fabs(p2.z - p1.z);
}

inline float scalarProduct(const Point& p1, const Point& p2) {
    return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
}

inline bool pointOnLine(const Point& a, const Point& b, const Point& p)
{
    return pointOnLine(fPoint(a.x, a.y), fPoint(b.x, b.y), fPoint(p.x, p.y)) && pointOnLine(fPoint(a.x, a.z), fPoint(b.x, b.z), fPoint(p.x, p.z)) && pointOnLine(fPoint(a.y, a.z), fPoint(b.y, b.z), fPoint(p.y, p.z));
}


inline bool inRectSize(const Point& a, const Point& v, const Point& p) {
    return (p.x >= a.x && p.x <= a.x + v.x && p.y >= a.y && p.y <= a.y + v.y && p.z >= a.z && p.z <= a.z + v.z);
}

/* obroty */
inline Point rotateZX(const Point& o, Point a, const float& k) {
    if (k == 0.0f)
        return a;

    a -= o;
    a = rotateZXAround0(a, k);
    a += o;

    return a;
}

inline Point rotateZXAround0(const Point& a, const float& k) {
    if (k == 0.0f || a == PointZero)
        return a;

    float sink = sinf(k);
    float cosk = cosf(k);
    return Point(a.x * cosk + a.z * sink, a.y, -a.x * sink + a.z * cosk);
}

inline Point rotateXY(const Point& o, Point a, const float& k) {
    if (k == 0.0f)
        return a;

    a -= o;
    a = rotateXYAround0(a, k);
    a += o;

    return a;
}

inline Point rotateXYAround0(const Point& a, const float& k) {
    if (k == 0.0f || a == PointZero)
        return a;

    float sink = sinf(k);
    float cosk = cosf(k);
    return Point(a.x * cosk - a.y * sink, a.x * sink + a.y * cosk, a.z);
}

inline Point rotateYZ(const Point& o, Point a, const float& k) {
    if (k == 0.0f)
        return a;

    a -= o;
    a = rotateYZAround0(a, k);
    a += o;

    return a;
}

inline Point rotateYZAround0(const Point& a, const float& k) {
    if (k == 0.0f || a == PointZero)
        return a;

    float sink = sinf(k);
    float cosk = cosf(k);
    return Point(a.x, a.y * cosk - a.z * sink, a.y * sink + a.z * cosk);
}

inline Point setSphericalRotation(const Point& o, const float& r, const float& k1, const float& k2) {
    Point p;
    float x = r * cos(k2);
    float y = r * sin(k2);
    p.x = x * cos(k1);
    p.z = -x * sin(k1);
    p.y = y;
    return p + o;
}

inline double getSphericalK2(const Point& o, const Point& p) {
    double rad = dist(o, p);
    double k2, sink2;

    sink2 = (p.y - o.y) / rad;
    if (sink2 > 1.0) sink2 = 1.0;
    else if (sink2 < -1.0) sink2 = -1.0;
    k2 = asin(sink2);
    return k2;
}

inline double getSphericalK1(const Point& o, const Point& p) {
    double rad = dist(o, p);
    double k1, sink2, sink1, cosk2, cosk1;

    sink2 = (p.y - o.y) / rad;
    if (sink2 > 1.0) sink2 = 1.0;
    else if (sink2 < -1.0) sink2 = -1.0;
    cosk2 = sqrt(1 - sink2 * sink2);
    if (cosk2 != 0.0) {
        sink1 = -(p.z - o.z) / (rad * cosk2);
        if (sink1 > 1.0) sink1 = 1.0;
        else if (sink1 < -1.0) sink1 = -1.0;
        k1 = fabs(asin(sink1));
        cosk1 = (p.x - o.x) / (rad * cosk2);

        if (sink1 > 0 && cosk1 > 0) k1 = k1;
        else if (sink1 > 0 && cosk1 < 0) k1 = -k1 + M_PI;
        else if (sink1 < 0 && cosk1 < 0) k1 = k1 + M_PI;
        else if (sink1 < 0 && cosk1 > 0) k1 = -k1 + 2 * M_PI;

        if (fabs(sink1) <= GEOMETRY_RAD_EPSILON && cosk1 > 0.0) k1 = 0.0;
        else if (fabs(sink1) <= GEOMETRY_RAD_EPSILON && cosk1 < 0.0) k1 = M_PI;
        else if (sink1 > 0 && fabs(cosk1) <= GEOMETRY_RAD_EPSILON) k1 = M_PI * 0.5;
        else if (sink1 < 0 && fabs(cosk1) <= GEOMETRY_RAD_EPSILON) k1 = M_PI * 1.5;
    } else k1 = 0.0;
    return k1;
}

inline float getAngle(const Point& O, const Point& A, const Point& B) {
    double a2 = dists(O, A);
    double b2 = dists(O, B);
    double c2 = dists(A, B);

    if (b2 == 0 || a2 == 0)
        return 0.0f;

    double cos_alfa = (a2 + b2 - c2) / (2 * sqrt(a2 * b2));
    setInRange(-1.0, cos_alfa, 1.0);

    double rad = acos(cos_alfa);

    return rad;
}

inline  Point setLengthToVector(const Point& a, const float& r) {
    return a*sqrt((r*r)/dists(a));
}

inline  bool arePointsNear(const Point& p1, const Point& p2) {
    return isZero(dist(p1, p2));
}

inline bool linesParallel(const Point& A, const Point& B, const Point& C, const Point& D) {
    float ABdist = dist(A, B);
    float CDdist = dist(C, D);

    return arePointsNear((D - C) * ABdist, (B - A) * CDdist) || arePointsNear((D - C) * ABdist, -(B - A) * CDdist);
}

inline fPKT pointWithoutZ(const PKT& pkt) {
    return fPKT(pkt.x, pkt.y);
}
inline PKT orthoToVec(const PKT& b1, const PKT& b2)
{
    return  Punkt(
            b1.y * b2.z - b2.y * b1.z,
            b1.z * b2.x - b2.z * b1.x,
            b1.x * b2.y - b2.x * b1.y
            );
}


// iPoint3D

inline iPoint3D operator-(const iPoint3D& p) {
    return iPoint3D(-p.x, -p.y, -p.z);
}

inline iPoint3D operator+(const iPoint3D& a, const iPoint3D& b) {
    return iPoint3D(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline iPoint3D operator-(const iPoint3D& a, const iPoint3D& b) {
    return iPoint3D(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline iPoint3D operator*(const iPoint3D& a, const int& b) {
    return iPoint3D(a.x * b, a.y * b, a.z * b);
}

inline iPoint3D operator/(const iPoint3D& a, const int& b) {
    return iPoint3D(a.x / b, a.y / b, a.z / b);
}

inline bool operator==(const iPoint3D& a, const iPoint3D& b) {
    return (a.x == b.x && a.y == b.y && a.z == b.z);
}

inline bool operator!=(const iPoint3D& a, const iPoint3D& b) {
    return !(a == b);
}

inline iPoint3D & operator+=(iPoint3D& a, const iPoint3D& b) {
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    return a;
}

inline iPoint3D & operator-=(iPoint3D& a, const iPoint3D& b) {
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
    return a;
}

inline iPoint3D & operator*=(iPoint3D& a, const int& b) {
    a.x *= b;
    a.y *= b;
    a.z *= b;
    return a;
}

inline iPoint3D & operator/=(iPoint3D& a, const int& b) {
    a.x /= b;
    a.y /= b;
    a.z /= b;
    return a;
}

}

#endif
