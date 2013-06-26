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

        /// 'hard' shape
        TYPE_HARD,

        /// 'soft' shape
        TYPE_SOFT
    };
    TYPE type;

    /// Model/Envelope index.
    int id;

    /// Model triangle index.
    int nr;
};

#endif // SOLIDGAME_H
