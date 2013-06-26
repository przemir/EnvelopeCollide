#include "Player.h"
#include "Engine.h"

extern Engine& en;

Player::Player() : vectorFrontLength(1.0f), vectorBackLength(0.0f), displayAtVecLength(0.0f)
{
    id = OBJECT_PLAYER;
}

void Player::render()
{
    /// If mouse hovers some result in collision result list, character model moves to position of collision.
    Point tra = front()*displayAtVecLength;
    Point p = pos + tra;
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(p.x, p.y, p.z);
    glRotatef(radXZ, 0, 1, 0);

    /// Renders character.
    model.render();
    glPopMatrix();

    if(en.getGui().showPlayerSolid != 0)
    {
        /// Renders character physical model.
        displayShape = (DRAW_TYPE)(en.getGui().showPlayerSolid-1);
        glPushMatrix();
        glTranslatef(tra.x, tra.y, tra.z);
        Object::render();
        glPopMatrix();
    }

    if(en.getGui().showMoveVector)
    {
        /// Renders movement vector.
        glDisable(GL_TEXTURE_2D);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glLineWidth(3.0f);
        Point up(0.0f, 3.0f, 0.0f);
        Point from = pos + up + front()*vectorBackLength;
        Point to = pos + up + front()*vectorFrontLength;
        glBegin(GL_LINES);
        glVertex3f(from.x, from.y, from.z);
        glVertex3f(to.x, to.y, to.z);
        glEnd();
        glLineWidth(1.0f);
    }
}

void Player::setDisplayOptions()
{
    if(en.getGui().showPlayerSolid != 0)
        displayShape = (DRAW_TYPE)(en.getGui().showPlayerSolid-1);
}


