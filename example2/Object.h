/// Author: Przemysław Bągard

#ifndef OBJECT_H
#define OBJECT_H

#include "Geometry/Point.h"
#include "Model.h"
#include "Geometry/shapes.h"
#include "CollisionSystem.h"
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

    /// 'Hard' shape - obstacle like for example wall; character cannot go through this obstacle.
    ShapeContainer hardShape;

    /// 'Soft' shape - program handles collision between character and object.
    ShapeContainer softShape;

    /// Object position in world.
    Point pos;

    /// Direction angle.
    float radXZ;

    /// Is graphical modele also a physical model (for 'hard' shapes) (is graphical model considered during collision detection)?
    bool useModelAsHardShape;

    /// Is object dynamic (if object changes position or dissapear/appear, variable's value should be true).
    bool dynamic;

    /// Shows 'hard' shapes.
    bool displayHardShape;

    /// Shows 'soft' shapes.
    bool displaySoftShape;

    /// Is object exists (nonexisting object is not visible and also is not an obstacle).
    bool exist;

    /// Id for object data holded in octree.
    std::pair<unsigned long int, Id> atTree;

    /// 'Soft' shape color.
    Color softShapeColor;

    /// 'Hard' shape color.
    Color hardShapeColor;

    /// Called during module initialization.
    void init();

    /// Renders object.
    virtual void render();

    /// Every frame routine.
    void processTime(float elapsed_time);

    /// Returns current coordinates of i-th point.
    Point getCurrentPoint(int i);

    /// Returns current coordinates of i-th physical model 'hard' shape.
    EnvelopeCollide::Mesh getCurrentHardShape(int i);

    /// Returns current coordinates of i-th physical model 'soft' shape.
    EnvelopeCollide::Mesh getCurrentSoftShape(int i);

    /// Returns current coordinates of physical model shape given as parameter.
    EnvelopeCollide::Mesh getCurrentShape(EnvelopeCollide::Mesh data);

    /// Direction vector.
    Point front();

    /// Renders physical model.
    void renderShapeContainer(const ShapeContainer& shape, const Color &color, DRAW_TYPE type = DRAW_TYPE_LINES, int filterSphere = 0);

    /// Handles situation, when object 'obj' enters 'this' object.
    virtual void enterAction(Object* obj);

    /// Handles situation, when object 'obj' is inside 'this' object.
    virtual void stayAction(Object* obj);

    /// Handles situation, when object 'obj' exits 'this' object.
    virtual void exitAction(Object* obj);
};

/// Distinguished id - empty
const Object::Id OBJECT_NONE = 0;

/// Distinguished id - character
const Object::Id OBJECT_PLAYER = 2;

/// Distinguished id - 'hard' shape, which appear, when character go into specific 'soft' shape.
const Object::Id OBJECT_HIDDEN_HARD_SHAPE = 10;

/// Distinguished id - 'hard' and 'soft' shape, which is hidden at the start of program.
const Object::Id OBJECT_HIDDEN_SHAPES = 11;

/// Distinguished id - starts group of identifiers, which indicates objects like static walls or obstacles (not used by application logic).
const Object::Id OBJECT_FIRST = 100;

/// Distinguished id - starts group of identifiers, which indicates walls closing world space.
const Object::Id OBJECT_FIRST_WALL = 8000;

#endif // OBJECT_H
