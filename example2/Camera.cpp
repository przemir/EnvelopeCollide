#include "Camera.h"
#include "Geometry/math.h"
#include <GL/glu.h>

Camera::Camera() {}

void Camera::initialize(const PKT &pos, const fPKT &rad, float r)
{
    this->pos = pos;
    this->vAngle = rad;
    this->r = r;
}

void Camera::setCamera()
{
    if(mode == NORMAL)
    {
        float rad = (float)degToRad(vAngle.x);
        PKT lookAt;

        lookAt.x = float(pos.x + 100*cos(rad));
        lookAt.z = float(pos.z + 100*sin(rad));

        rad = degToRad(vAngle.y);

        lookAt.y = float (pos.y + 100 * sin(rad));
        rad = degToRad(vAngle.x);

        gluLookAt(pos.x, pos.y, pos.z,
                  lookAt.x, lookAt.y, lookAt.z,  0.0, 1.0, 0.0);
    }
    else if(mode == LOOKING_AT)
    {
        PKT lookAt = setSphericalRotation(pos, 0.0001, degToRad(vAngle.x), degToRad(vAngle.y));
        PKT cameraPos = setSphericalRotation(pos, r, degToRad(vAngle.x), degToRad(vAngle.y));
        gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z,
                  lookAt.x, lookAt.y, lookAt.z,  0.0, 1.0, 0.0);
    }
}
