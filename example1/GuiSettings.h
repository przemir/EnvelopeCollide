#ifndef GUISETTINGS_H
#define GUISETTINGS_H

#include <QTextStream>
#include "Geometry/Point.h"

using namespace EnvelopeGeometry;

/// Rendering/collision detections properties.
struct GuiSettings {
    /// Shows object envelope (wireframe envelope/filled envelope).
    int showObjectSolid;

    /// Shows character envelope (no/wireframe envelope/filled envelope).
    int showPlayerSolid;

    /// Shows movement vector.
    bool showMoveVector;

    /// Shows ground.
    bool showPlane;

    /// Collision interpretation mode (all collisions/first collision).
    int logMode;

    /// Application logic criteria - which shape we consider in collision detection routine (all/green/not green).
    int criteriaMode;

    /// Writes stream.
    friend QTextStream &operator << (QTextStream& stream, const GuiSettings& myclass);

    /// Reads stream.
    friend QTextStream &operator >> (QTextStream& stream, GuiSettings& myclass);
};

QTextStream &operator << (QTextStream& stream, const GuiSettings& myclass);
QTextStream &operator >> (QTextStream& stream, GuiSettings& myclass);

#endif // GUISETTINGS_H
