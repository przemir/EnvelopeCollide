#include "Controllers.h"
#include "QFile"

Controllers::Controllers()
{
    keyRotateLeft = Qt::Key_Left;
    keyRotateRight = Qt::Key_Right;
    keyUp = Qt::Key_Up;
    keyDown = Qt::Key_Down;
}

void Controllers::setKeyPressed(int key)
{
    _keys.insert(key);
    _curr_pressed.insert(key);
}

void Controllers::setKeyUnPressed(int key)
{
    _keys.remove(key);
    _curr_released.insert(key);
}

bool Controllers::keyIsPressed(int key)
{
    return _keys.contains(key);
}

bool Controllers::keyPressed(int key)
{
    return _curr_pressed.contains(key);
}

bool Controllers::keyReleased(int key)
{
    return _curr_released.contains(key);
}

void Controllers::clearTurn()
{
    _curr_pressed.clear();
    _curr_released.clear();
}
