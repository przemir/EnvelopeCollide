/// Author: Przemysław Bągard

#ifndef CONTROLLERS_H
#define CONTROLLERS_H

#include "QSet"

/// Handles keyboard and manage program keys binding.
class Controllers
{
public:
    Controllers();

    /// Key was pressed.
    void setKeyPressed(int key);

    /// Key was released.
    void setKeyUnPressed(int key);

    /// Is key pressed?
    bool keyIsPressed(int key);

    /// Is key pressed in this frame?
    bool keyPressed(int key);

    /// Is key released in this frame?
    bool keyReleased(int key);

    /// Clear information about pressed/released keys in this frame (clear list of key events).
    void clearTurn();

    /** Binded keys */

    /// Turn left
    int keyRotateLeft;

    /// Turn right
    int keyRotateRight;

    /// Move forward
    int keyUp;

    /// Go back
    int keyDown;

private:
    /// Pressed keys.
    QSet<int> _keys;

    /// Pressed in this frame key.
    QSet<int> _curr_pressed;

    /// Released in this frame key.
    QSet<int> _curr_released;
};

#endif // CONTROLLERS_H
