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

    /// Sets character envelope rendering options.
    virtual void setDisplayOptions();

    /// Length of movement vector (from character position to latest front position, where collisions are considered) - renders line segment.
    float vectorFrontLength;

    /// Length of movement vector (from character position to latest back position, where collisions are considered) - renders line segment.
    float vectorBackLength;

    /// Position of character ('pos + displayAtVecLength*vec', where 'vec' is moment, when collision occurs).
    /// Value changes when mouse is hovered at collision detection result list.
    float displayAtVecLength;

    /** shapes for physical model of character */

    /// Sphere.
    EnvelopeCollide::Mesh pointShape;

    /// Line segment envelope.
    EnvelopeCollide::Mesh segmentShape;

    /// Triangle envelope.
    EnvelopeCollide::Mesh triangleShape;
};

#endif // PLAYER_H
