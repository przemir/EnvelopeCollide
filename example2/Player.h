/// Author: Przemysław Bągard

#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"

/// Character
class Player : public Object
{
public:
    Player();

    /// Renders character.
    virtual void render();

    /// Length of movement vector (from character position to latest front position, where collisions are considered) - renders line segment.
    float vectorLength;

    /// Length of movement vector (from character position to latest back position, where collisions are considered) - renders line segment.
    float vectorBack;

    /// Has character his full speed (character may be slow due to specific soft shape)?
    bool fullspeed;
};

/// Animation state - standing.
const int ANIMATION_STAND = 0;

/// Animation state - moving.
const int ANIMATION_MOVE = 2;

#endif // PLAYER_H
