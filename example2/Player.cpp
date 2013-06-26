#include "Player.h"
#include "Engine.h"

extern Engine& en;

Player::Player() : vectorLength(1.0f), vectorBack(0.0f), fullspeed(true)
{
    id = OBJECT_PLAYER;
}

void Player::render()
{
    /// Don't display character envelope
    displayHardShape = false;
    Object::render();
}


