#ifndef SITUATION_H
#define SITUATION_H

#include <QTextStream>
#include "GuiSettings.h"
#include "Geometry/Point.h"
#include "Geometry/Point2D.h"

using namespace EnvelopeGeometry;

/// Situation data
struct Situation {
    /// Saved gui settings (rendering/collision detection options) for this situation.
    GuiSettings gui;

    /// Character position.
    PKT pos;

    /// Character direction angle.
    float rad;

    /// Camera position.
    PKT camera;

    /// Camera direction angles.
    fPKT cameraRad;

    /// Length of movement vector (from character position to latest front position, where collisions are considered).
    float frontVecLenght;

    /// Length of movement vector (from character position to latest back position, where collisions are considered).
    float backVecLenght;

    /// Player physical model shape (sphere, line segment envelope, triangle envelope).
    int playerShape;

    /// How long this situation will last.
    float time;

    friend QTextStream &operator << (QTextStream& stream, const Situation& myclass);
    friend QTextStream &operator >> (QTextStream& stream, Situation& myclass);
};

QTextStream &operator << (QTextStream& stream, const Situation& myclass);
QTextStream &operator >> (QTextStream& stream, Situation& myclass);

#endif // SITUATION_H
