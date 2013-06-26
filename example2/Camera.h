/// Author: Przemysław Bągard

#ifndef CAMERA_INCLUDED
#define CAMERA_INCLUDED

#include "Geometry/Point.h"
#include "Geometry/Point2D.h"

using namespace EnvelopeGeometry;

/// Camera - usage of openGL functions.
class Camera
{
public:
    Camera();

    /// Camera modes.
    enum Mode {
        /// Normal: position and angles.
        NORMAL,
        /// Look at: center point, distance and angles.
        LOOKING_AT
    };

    /// Sets camera data
    void initialize(const PKT& pos = PKT0, const fPKT& rad = fPKT0, float r = 1.0f);

    /// Execute 'gluLookAt' for camera data
    void setCamera();

    /** Camera data */

    /// Camera mode
    Mode mode;

    /// Position
    PKT pos;

    /// Angles: XZ and inclination
    fPKT vAngle;

    /// Distance from target
    float r;
};

#endif
