/* 
 * File:   math.h
 * Author: Przemysław Bągard
 *
 * Created on 14 wrzesień 2010, 10:21
 */

#ifndef GEOMETRY_MATH_H
#define	GEOMETRY_MATH_H

#include <cstdlib>
#include <cmath>
#include "const.h"

/// math functions
namespace EnvelopeGeometry {

/// conversion: degrees to radians
inline double degToRad(const double& k);

/// conversion: radians to degrees
inline double radToDeg(const double& k);

/// sets 'k' angle to equivalent in the range [0,360) degrees (parameter and result in degrees!)
inline double normalizeAngle(double k);

/// swap min and max if the 'min' parameter is greater from 'max'
template <typename T>
inline void setMinMax(T& min, T& max);

/// if parameter 'value' is beyond the interval ['min', 'max'], it sets it to closer value from those two parameters
template <typename T>
inline void setInRange(T min, T& value, T max);

/// absolute value
inline float abs(const float& value);

/// returns minimal value from given parameters
template <typename T>
inline T min(const T& a, const T& b);

/// returns maximal value from given parameters
template <typename T>
inline T max(const T& a, const T& b);

/// returns true if parameter 'value' is negative (including epsilon)
template <typename T>
inline bool isNegative(const T& value);

/// returns true if parameter 'value' equals zero (including epsilon)
template <typename T>
inline bool isZero(const T& value);

/// returns true if parameter 'value' is positive (including epsilon)
template <typename T>
inline bool isPositive(const T& value);

/// approximate parameter 'value' to closest multiple of the parameter 'step'
template <typename T>
inline T approximate(const T& value, const T& step);
}

/******************************* implementation *******************************/

namespace EnvelopeGeometry {

inline double degToRad(const double& k) { return ((k*M_PI)/180.0); }
inline double radToDeg(const double& k) { return ((k/M_PI)*180.0); }

inline double normalizeAngle(double k) {
    k = fmod(k, 360);

    if (k < 0) {
        k = fmod(k + 360, 360);
    }

    return k;
}

template <typename T>
inline void setMinMax(T& min, T& max)
{
    if(max < min)
    {
        T temp = min;
        min = max;
        max = temp;
    }
}

template <typename T>
inline void setInRange(T min, T& value, T max) {
    if (value < min) {
        value = min;
    } else if (max < value) {
        value = max;
    }
}

inline float abs(const float& value) {
    return fabs(value);
}

template <typename T>
inline T min(const T& a, const T& b) {
    if (a < b)
        return a;
    else
        return b;
}

template <typename T>
inline T max(const T& a, const T& b) {
    if (a > b)
        return a;
    else
        return b;
}

template <>
inline bool isNegative(const float& value) {
    return value < -GEOMETRY_FLOAT_EPSILON;
}

template <>
inline bool isZero(const float& value) {
    return abs(value) <= GEOMETRY_FLOAT_EPSILON;
}

template <>
inline bool isPositive(const float& value) {
    return value > GEOMETRY_FLOAT_EPSILON;
}

template <>
inline bool isNegative(const int& value) {
    return value < 0;
}

template <>
inline bool isZero(const int& value) {
    return value == 0;
}

template <>
inline bool isPositive(const int& value) {
    return value > 0;
}

template <typename T>
inline T approximate(const T& value, const T& step)
{
    T r = fmod(value, step);
    T v = (int)(value / step);
    v = v*step;
    if(value >= 0)
    {
        if(r >= step/2) v += step;
    }
    else
    {
        if(r <= -step/2) v -= step;
    }
    return v;
}

}

#endif	/* GEOMETRY_MATH_H */

