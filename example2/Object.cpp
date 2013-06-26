#include "Object.h"
#include "GL/gl.h"
#include "Geometry/shapes.h"
#include "QDebug"

Object::Object() : radXZ(0.0f), useModelAsHardShape(true), dynamic(false), displayHardShape(true), displaySoftShape(true), exist(true), softShapeColor(0.0f, 1.0f, 0.0f), hardShapeColor(1.0f, 0.0f, 0.0f)
{
}

void Object::init()
{
    model.initialize();
    model.calculate();
}

void Object::render()
{
    if(exist)
    {
        /// Object exists.
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);
        glRotatef(radXZ, 0, 1, 0);
        /// Renders object.
        model.render();
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);
        /// Displays 'hard' shapes.
        if(displayHardShape)
        {
            hardShapeColor.use();
            renderShapeContainer(hardShape, hardShapeColor, DRAW_TYPE_FILLED);
        }

        /// Displays 'soft' shapes.
        if(displaySoftShape)
        {
            softShapeColor.use();
            renderShapeContainer(softShape, softShapeColor, DRAW_TYPE_LINES);
        }
    }
}

void Object::processTime(float elapsed_time)
{
    model.processTime(elapsed_time);
}

Point Object::getCurrentPoint(int i)
{
    /// Transforms coordinates from loaded model to coordinates for current moment.
    Point p(model.getModelData().vertex[3*i+0], model.getModelData().vertex[3*i+1], model.getModelData().vertex[3*i+2]);
    p = rotateZXAround0(p, degToRad(radXZ));
    p += pos;
    return p;
}

EnvelopeCollide::Mesh Object::getCurrentHardShape(int i)
{
    EnvelopeCollide::Mesh data = hardShape[i];
    return getCurrentShape(data);
}

EnvelopeCollide::Mesh Object::getCurrentSoftShape(int i)
{
    EnvelopeCollide::Mesh data = softShape[i];
    return getCurrentShape(data);
}

EnvelopeCollide::Mesh Object::getCurrentShape(EnvelopeCollide::Mesh data)
{
    /// Transforms coordinates from loaded model to coordinates for current moment.
    for (int l1 = 0; l1 < data.pCount(); ++l1) {
        data.p[l1] = rotateZXAround0(data.p[l1], degToRad(radXZ));
        data.p[l1] += pos;
    }
    return data;
}

void Object::enterAction(Object *obj)
{
    qDebug() << "obiekt[" << obj->id << "] wszedl w obiekt[" << id << "]";
}

void Object::stayAction(Object *obj)
{
    qDebug() << "obiekt[" << obj->id << "] wciaz jest wewnatrz obiektu[" << id << "]";
}

void Object::exitAction(Object *obj)
{
    qDebug() << "obiekt[" << obj->id << "] opuszcza obiekt[" << id << "]";
}

Point Object::front()
{
    return rotateZXAround0(Point(1.0, 0.0, 0.0), degToRad(radXZ));
}

void Object::renderShapeContainer(const Object::ShapeContainer &shape, const Color &color, DRAW_TYPE type, int filterSphere)
{
    /// Dark color - used for outlines (wireframe shape) if shape render mode is 'filled'.
    Color darkColor = Color(color.r/2.0f, color.g/2.0f, color.b/2.0f);
    /// Wireframed shape has greater radius.
    float eps = 0.002f;
    for (int l1 = 0; l1 < shape.size(); ++l1) {
        EnvelopeCollide::Mesh data = getCurrentShape(shape[l1]);
        color.use();
        switch(data.type)
        {
        case EnvelopeCollide::POINT:
            drawSphere(data.p[0], 8, 8, data.r, type);
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
