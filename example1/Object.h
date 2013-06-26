/// Author: Przemysław Bągard

#ifndef OBJECT_H
#define OBJECT_H

#include "Geometry/Point.h"
#include "Model.h"
#include "CollisionSystem/Mesh.h"
#include "Geometry/shapes.h"
#include "Color.h"

using namespace EnvelopeGeometry;

/// Objects occuring in module.
class Object
{
public:
    Object();

    /// Unique object id.
    typedef unsigned int Id;

    /// Type of container holding shapes.
    typedef QList<EnvelopeCollide::Mesh> ShapeContainer;

    /// Object id
    Id id;

    /// Graphical model
    Model model;

    /// Shape mode, which belongs to object.
    enum SHAPE_TYPE {
        /// Normal shape.
        SHAPE_NORMAL,

        /// Special shape.
        SHAPE_SPECIAL
    } shapeType;

    /// Physical model.
    ShapeContainer shape;

    /// Object position in world.
    Point pos;

    /// Direction angle.
    float radXZ;

    /// Is graphical modele also a physical model (is graphical model considered during collision detection)?
    bool useModelAsShape;

    /// Render mode of shapes (wireframe/filled).
    DRAW_TYPE displayShape;

    /// Shape color.
    Color shapeColor;

    /// Called during module initialization.
    void init();

    /// Renders object.
    virtual void render();

    /// Sets render mode of shape from engine settings.
    virtual void setDisplayOptions();

    /// Every frame routine.
    void processTime(float elapsed_time);

    /// Returns current coordinates of i-th point.
    Point getCurrentPoint(int i);

    /// Returns current coordinates of i-th physical model shape.
    EnvelopeCollide::Mesh getCurrentShape(int i);

    /// Returns current coordinates of physical model shape given as parameter.
    EnvelopeCollide::Mesh getCurrentMeshShape(EnvelopeCollide::Mesh data);

    /// Direction vector.
    Point front();

    /// Renders physical model.
    void renderShapeContainer(const ShapeContainer& shape, const Color &color, DRAW_TYPE type = DRAW_TYPE_LINES);
};

/// Distinguished id - empty
const Object::Id OBJECT_NONE = 0;

/// Distinguished id - character
const Object::Id OBJECT_PLAYER = 2;

/// Distinguished id - starts group of identifiers, which indicates objects like static walls or obstacles (not used by application logic).
const Object::Id OBJECT_FIRST = 3;

#endif // OBJECT_H
