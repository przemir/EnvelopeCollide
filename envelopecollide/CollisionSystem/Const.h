/// Author: Przemysław Bągard

#ifndef CONST_H
#define CONST_H

/// constants
namespace EnvelopeCollide
{
    /// epsilon for collision detection radius
    const float COLLISION_MIN_R = 0.001f;

    /// shape of object
    enum SHAPE
    {
        POINT,
        LINE,
        TRIANGLE
    };

    /// events for objects
    enum EVENT_MODE
    {
        /// enter object
        EENTER,
        /// is inside object
        EINSIDE,
        /// exit object
        EEXIT
    };
}

#endif // CONST_H
