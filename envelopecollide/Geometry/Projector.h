/* 
 * File:   Projector.h
 * Author: Przemysław Bągard
 *
 *
 * Created on April 19, 2012, 1:05 PM
 */

#ifndef PROJECTOR_H
#define	PROJECTOR_H

#include "Point.h"
#include "Point2D.h"

namespace EnvelopeGeometry {

/// intended to be used to transform point coordinate from one coordinate system to another (for example to project point to 2D plane)
class Projector
{
public:
    /// constructor with: 'center' - center of new origin of coordinate system, 'bx', 'by' - vectors which creates coordinate system (third vector is perpendicular to vectors 'bx', 'by')
    inline Projector(const PKT& center, const PKT& bx, const PKT& by);

    /// constructor with: 'center' - center of new origin of coordinate system, 'bx', 'by', 'bz' - vectors which creates coordinate system
    inline Projector(const PKT& center, const PKT& bx, const PKT& by, const PKT& bz);

    /// returns point in original coordinate system
    inline PKT  detransform(const PKT& p) const;

    /// returns point in projector coordinate system
    inline PKT  transform(const PKT& p) const;

    /// like detransform, but without 'z' axis
    inline PKT  detransformWithoutZ(const fPKT& p) const;

    /// like transform, but without 'z' axis
    inline fPKT transformWithoutZ(const PKT& p) const;

    /// checks if projector is valid (projector is not valid if determinant equals zero) (including epsilon)
    inline bool isValid() const;

    /// returns origin of coordinate system
    inline PKT getCenter() const;

    /// returns i-th vector of coordinate system
    inline PKT getBase(int i) const;

    /// returns determinant of projector matrix
    inline float getW() const;

    /// sets origin of coordinate system
    inline void setCenter(const PKT &p);

private:
    PKT center;     /// origin of coordinate system
    PKT base[3];    /// base vectors
    float W;        /// determinant of projector matrix

    /// determinants 2x2, created during projector initializantion
    float wax,way,waz, wbx,wby,wbz, wcx,wcy,wcz;

    inline float det2d(float ax, float bx, float ay, float by);
    inline void detCalculate(const PKT& a, const PKT& b, const PKT& c);
};

}

/// implementation
namespace EnvelopeGeometry {


inline Projector::Projector(const PKT& center, const PKT& bx, const PKT& by)
{
    this->center = center;
    base[0] = bx;
    base[1] = by;
    base[2] = orthoToVec(bx, by);
    detCalculate(bx, by, base[2]);
}

inline Projector::Projector(const PKT& center, const PKT& bx, const PKT& by, const PKT& bz)
{
    this->center = center;
    base[0] = bx;
    base[1] = by;
    base[2] = bz;
    detCalculate(bx, by, bz);
}

inline void Projector::detCalculate(const PKT& a, const PKT& b, const PKT& c)
{
    W = det(a, b, c);
    wax = det2d(b.y, b.z, c.y, c.z);
    way = det2d(b.z, b.x, c.z, c.x);
    waz = det2d(b.x, b.y, c.x, c.y);

    wbx = det2d(a.z, a.y, c.z, c.y);
    wby = det2d(a.x, a.z, c.x, c.z);
    wbz = det2d(a.y, a.x, c.y, c.x);

    wcx = det2d(a.y, a.z, b.y, b.z);
    wcy = det2d(a.z, a.x, b.z, b.x);
    wcz = det2d(a.x, a.y, b.x, b.y);
}


inline float Projector::det2d(float ax, float bx,
                              float ay, float by)
{
    return ax*by - ay*bx;
}

inline PKT  Projector::detransformWithoutZ(const fPKT& p) const
{
    return center + base[0]*p.x + base[1]*p.y;
}

inline fPKT Projector::transformWithoutZ(const PKT& p) const
{
    PKT value = transform(p);
    return fPunkt(value.x, value.y);
}

inline PKT  Projector::detransform(const PKT& p) const
{
    return center + base[0]*p.x + base[1]*p.y + base[2]*p.z;
}

inline PKT  Projector::transform(const PKT& p) const
{
    PKT target = p-center;
//  equation: base[0] * zm.x + base[1] * zm.y + base[2] * zm.z = target
    float Wt1 = target.x*wax + target.y*way + target.z*waz;
    float Wt2 = target.x*wbx + target.y*wby + target.z*wbz;
    float Wt3 = target.x*wcx + target.y*wcy + target.z*wcz;

    return Punkt(Wt1/W, Wt2/W, Wt3/W);
}

inline void Projector::setCenter(const PKT& p)
{
    center = p;
}

inline bool Projector::isValid() const { return !isZero(W); }
inline PKT Projector::getCenter() const { return center; }
inline PKT Projector::getBase(int i) const { return base[i]; }
inline float Projector::getW() const { return W; }

}

#endif	/* PROJECTOR_H */

