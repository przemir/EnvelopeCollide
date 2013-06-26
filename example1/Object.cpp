#include "Object.h"
#include "Geometry/shapes.h"
#include "QDebug"
#include "Engine.h"

extern Engine& en;

Object::Object() : shapeType(SHAPE_NORMAL), radXZ(0.0f), useModelAsShape(true), displayShape(DRAW_TYPE_FILLED), shapeColor(1.0f, 0.0f, 0.0f)
{
}

void Object::init()
{
    model.initialize();
    model.calculate();
}

void Object::render()
{
    setDisplayOptions();
    glDisable(GL_TEXTURE_2D);

    /// Renders shape
    shapeColor.use();
    renderShapeContainer(shape, shapeColor, displayShape);
}

void Object::setDisplayOptions()
{
    displayShape = (DRAW_TYPE)en.getGui().showObjectSolid;
}

void Object::processTime(float elapsed_time)
{
    model.processTime(elapsed_time);
}

Point Object::getCurrentPoint(int i)
{
    Point p(model.getModelData().vertex[3*i+0], model.getModelData().vertex[3*i+1], model.getModelData().vertex[3*i+2]);
    p = rotateZXAround0(p, degToRad(radXZ));
    p += pos;
    return p;
}

EnvelopeCollide::Mesh Object::getCurrentShape(int i)
{
    EnvelopeCollide::Mesh data = shape[i];
    return getCurrentMeshShape(data);
}

EnvelopeCollide::Mesh Object::getCurrentMeshShape(EnvelopeCollide::Mesh data)
{
    for (int l1 = 0; l1 < data.pCount(); ++l1) {
        data.p[l1] = rotateZXAround0(data.p[l1], degToRad(radXZ));
        data.p[l1] += pos;
    }
    return data;
}

Point Object::front()
{
    return rotateZXAround0(Point(1.0, 0.0, 0.0), degToRad(radXZ));
}

void Object::renderShapeContainer(const Object::ShapeContainer &shape, const Color& color, DRAW_TYPE type)
{
    /// Dark color - used for outlines (wireframe shape) if shape render mode is 'filled'.
    Color darkColor = Color(color.r/2.0f, color.g/2.0f, color.b/2.0f);
    /// Wireframed shape has greater radius.
    float eps = 0.002f;
    for (int l1 = 0; l1 < shape.size(); ++l1) {
        EnvelopeCollide::Mesh data = getCurrentMeshShape(shape[l1]);
        color.use();
        switch(data.type)
        {
        case EnvelopeCollide::POINT:
            drawSphere(data.p[0], 16, 16, data.r, type);
            if(type == DRAW_TYPE_FILLED)
            {
                darkColor.use();
                drawSphere(data.p[0], 16, 16, data.r+eps, DRAW_TYPE_LINES);
            }
            break;
        case EnvelopeCollide::LINE:
            drawQuasiLine(data.p[0], data.p[1], 8, 8, data.r, type);
            if(type == DRAW_TYPE_FILLED)
            {
                darkColor.use();
                drawQuasiLine(data.p[0], data.p[1], 8, 8, data.r+eps, DRAW_TYPE_LINES);
            }
            break;
        case EnvelopeCollide::TRIANGLE:
            drawQuasiTriangle(data.p[0], data.p[1], data.p[2], 8, 8, data.r, type);
            if(type == DRAW_TYPE_FILLED)
            {
                darkColor.use();
                drawQuasiTriangle(data.p[0], data.p[1], data.p[2], 8, 8, data.r+eps, DRAW_TYPE_LINES);
            }
            break;
        default:
            break;
        }
    }
}

