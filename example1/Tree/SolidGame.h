/// Author: Przemysław Bągard

#ifndef SOLIDGAME_H
#define SOLIDGAME_H

/// Object data stored in octree.
struct SolidGame
{
    /// Physical model mode.
    enum TYPE{
        /// Generic object - graphical object is also physical model.
        TYPE_GENERIC,

        /// Normal shape for object.
        TYPE_NORMAL,

        /// Distinguished shape for object
        /// (difference between TYPE_NORMAL and TYPE_SPECIAL is only to apply application logic criteria).
        TYPE_SPECIAL
    } type;

    /// Model/Envelope index.
    int id;

    /// Model triangle index.
    int nr;
};

#endif // SOLIDGAME_H
