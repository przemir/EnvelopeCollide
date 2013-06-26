#include "ModuleCreation.h"
#include "Module.h"
#include "ModelData.h"      /// character model
#include "ModelData2.h"     /// box model
#include "QImage"
#include "Engine.h"
#include "Player.h"
#include <QDebug>

extern Engine& en;

ModuleCreation::ModuleCreation()
{
}

void ModuleCreation::create(Module *m)
{
    {
        /// Character.

        Player* obj = new Player();

        /// Loads model and animation.
        obj->model.setModel(model_vertex, model_texCoord, model_tri, 1144, 404);
        obj->model.setAnimation(animation_frame_start, 6, 0.25f);

        /// Player envelope is blue.
        obj->shapeColor = Color(0.0f, 0.0f, 1.0f);

        /// Physical model of character is chosen by user.
        /// Hovewer all of them are created now (sphere, line segment envelope, triangle envelope).
        obj->triangleShape.type = EnvelopeCollide::TRIANGLE;
        obj->triangleShape.r = 1.0f;
        obj->triangleShape.p[0] = Point(0, 1.0, 0);
        obj->triangleShape.p[1] = Point(0, 3.25, 0);
        obj->triangleShape.p[2] = Point(1, 3.5, 0);

        obj->segmentShape.type = EnvelopeCollide::LINE;
        obj->segmentShape.r = 1.0f;
        obj->segmentShape.p[0] = Point(0, 1.0, 0);
        obj->segmentShape.p[1] = Point(0, 3.25, 0);
        obj->segmentShape.p[2] = Point(1, 3.5, 0);

        obj->pointShape.type = EnvelopeCollide::POINT;
        obj->pointShape.r = 2.0f;
        obj->pointShape.p[0] = Point(0, 2.0, 0);
        obj->pointShape.p[1] = Point(0, 3.25, 0);
        obj->pointShape.p[2] = Point(1, 3.5, 0);

        obj->shape.push_back(obj->segmentShape);

        /// Loads texture from file.
        QImage image;
        if(!image.load("tex.bmp"))
        {
            qDebug() << "Texture file cannot be loaded: " << "tex.bmp";
            qDebug() << image.text();
        }
        obj->model.createTexture(image);

        /// Adds object to module.
        m->add(obj);
    }

    /// Creates next objects, indexed sequentially, starting from 'OBJECT_FIRST' value.
    int licz = OBJECT_FIRST;
    {
        /// Normal red shape

        Object* obj = new Object();
        obj->id = licz;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(8, 0, 8);
        obj->shape.push_back(EnvelopeCollide::Mesh());
        obj->shape[0].r = 1.0f;
        obj->shape[0].type = EnvelopeCollide::LINE;
        obj->shape[0].p[0] = Point(0, 1, 0);
        obj->shape[0].p[1] = Point(0, 10, 0);

        /// Adds object to module.
        m->add(obj);

        /// Next object will have next index.
        ++licz;
    }
    {
        /// Distinguished, green shape (Application logic criteria may omit some objects).

        Object* obj = new Object();
        obj->id = licz;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(12, 0, 8);
        obj->shapeType = Object::SHAPE_SPECIAL;
        obj->shapeColor = Color(0.0f, 1.0f, 0.0f);
        obj->shape.push_back(EnvelopeCollide::Mesh());
        obj->shape[0].r = 1.0f;
        obj->shape[0].type = EnvelopeCollide::LINE;
        obj->shape[0].p[0] = Point(0, 0, 0);
        obj->shape[0].p[1] = Point(0, 6, 0);

        /// Adds object to module.
        m->add(obj);
        ++licz;
    }

    /// Normal red shapes
    {
        Object* obj = new Object();
        obj->id = licz;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(-4, 0, -2);
        obj->shape.push_back(EnvelopeCollide::Mesh());
        obj->shape[0].r = 1.0f;
        obj->shape[0].type = EnvelopeCollide::TRIANGLE;//obj->hardShape[0].type = Solid::LINE;
        obj->shape[0].p[0] = Point(0, 1, 0);
        obj->shape[0].p[1] = Point(0, 4, 1);
        obj->shape[0].p[2] = Point(1, 3, 0);

        /// Adds object to module.
        m->add(obj);
        ++licz;
    }

    {
        Object* obj = new Object();
        obj->id = licz;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(7, 0, -2);
        obj->shape.push_back(EnvelopeCollide::Mesh());
        obj->shape[0].r = 1.0f;
        obj->shape[0].type = EnvelopeCollide::LINE;
        obj->shape[0].p[0] = Point(0, 0.5, 0);
        obj->shape[0].p[1] = Point(0, 0.5, 5);

        m->add(obj);
        ++licz;
    }

    {
        Object* obj = new Object();
        obj->id = licz;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(12, 0, -8);
        obj->shape.push_back(EnvelopeCollide::Mesh());
        obj->shape[0].r = 0.25f;
        obj->shape[0].type = EnvelopeCollide::POINT;//obj->hardShape[0].type = Solid::LINE;
        obj->shape[0].p[0] = Point(0, 0.0, 0);

        m->add(obj);
        ++licz;
    }

    {
        Object* obj = new Object();
        obj->id = licz;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(-2, 0, 11);
        obj->shape.push_back(EnvelopeCollide::Mesh());
        obj->shape[0].r = 0.3f;
        obj->shape[0].type = EnvelopeCollide::LINE;
        obj->shape[0].p[0] = Point(-1, -2.0, 0);
        obj->shape[0].p[1] = Point(4, 2.0, 0);

        m->add(obj);
        ++licz;
    }

    {
        Object* obj = new Object();
        obj->id = licz;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(-2, 0, 14);
        obj->shape.push_back(EnvelopeCollide::Mesh());
        obj->shape[0].r = 0.5f;
        obj->shape[0].type = EnvelopeCollide::LINE;
        obj->shape[0].p[0] = Point(-1, -1.0, 0);
        obj->shape[0].p[1] = Point(1, 1.0, 0);

        m->add(obj);
        ++licz;
    }

    {
        Object* obj = new Object();
        obj->id = licz;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(8, 0, 14);
        obj->shape.push_back(EnvelopeCollide::Mesh());
        obj->shape[0].r = 0.75f;
        obj->shape[0].type = EnvelopeCollide::TRIANGLE;
        obj->shape[0].p[0] = Point(-1, -1.0, 0);
        obj->shape[0].p[1] = Point(-1, 1.0, 0);
        obj->shape[0].p[2] = Point(1, -1.0, 0);

        m->add(obj);
        ++licz;
    }


    {
        Object* obj = new Object();
        obj->id = licz;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(12, 0, 17);
        obj->shape.push_back(EnvelopeCollide::Mesh());
        obj->shape[0].r = 0.75f;
        obj->shape[0].type = EnvelopeCollide::TRIANGLE;
        obj->shape[0].p[0] = Point(-1, 0.0, 0);
        obj->shape[0].p[1] = Point(-3, 2.0, 0);
        obj->shape[0].p[2] = Point(2, -1.25, 0);

        m->add(obj);
        ++licz;
    }


    {
        Object* obj = new Object();
        obj->id = licz;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(18, 0, 4);
        obj->shape.push_back(EnvelopeCollide::Mesh());
        obj->shape[0].r = 1.75f;
        obj->shape[0].type = EnvelopeCollide::POINT;
        obj->shape[0].p[0] = Point(0.0f, 0.0, 0);
        obj->shape[0].p[1] = Point(-3, 2.0, 0);
        obj->shape[0].p[2] = Point(2, -1.25, 0);

        m->add(obj);
        ++licz;
    }

    {
        /// Object, which physical model consists of multiple shapes.
        /// If character collides with any of those shapes, it collides with object.
        /// Shapes have orange color. Object physical model looks like box.

        Object* obj = new Object();
        obj->shapeColor = Color(1.0f, 0.5f, 0.0f);
        obj->id = licz;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(24, 0, -6);
        float sx = 0.5f;
        float sy = 0.5f;
        float sz = 1.5f;
        float r = 0.005f;
        obj->shape.push_back(EnvelopeCollide::Mesh());
        obj->shape[0].r = r;
        obj->shape[0].type = EnvelopeCollide::TRIANGLE;
        obj->shape[0].p[0] = Point(-sx, -sy, -sz);
        obj->shape[0].p[1] = Point( sx, -sy, -sz);
        obj->shape[0].p[2] = Point(-sx,  sy, -sz);


        obj->shape.push_back(EnvelopeCollide::Mesh());
        obj->shape[1].r = r;
        obj->shape[1].type = EnvelopeCollide::TRIANGLE;
        obj->shape[1].p[0] = Point( sx,  sy, -sz);
        obj->shape[1].p[1] = Point( sx, -sy, -sz);
        obj->shape[1].p[2] = Point(-sx,  sy, -sz);

        obj->shape.push_back(EnvelopeCollide::Mesh());
        obj->shape[2].r = r;
        obj->shape[2].type = EnvelopeCollide::TRIANGLE;
        obj->shape[2].p[0] = Point(-sx, -sy,  sz);
        obj->shape[2].p[1] = Point( sx, -sy,  sz);
        obj->shape[2].p[2] = Point(-sx,  sy,  sz);

        obj->shape.push_back(EnvelopeCollide::Mesh());
        obj->shape[3].r = r;
        obj->shape[3].type = EnvelopeCollide::TRIANGLE;
        obj->shape[3].p[0] = Point( sx,  sy,  sz);
        obj->shape[3].p[1] = Point( sx, -sy,  sz);
        obj->shape[3].p[2] = Point(-sx,  sy,  sz);

        obj->shape.push_back(EnvelopeCollide::Mesh());
        obj->shape[4].r = r;
        obj->shape[4].type = EnvelopeCollide::TRIANGLE;
        obj->shape[4].p[0] = Point(-sx, -sy, -sz);
        obj->shape[4].p[1] = Point( sx, -sy, -sz);
        obj->shape[4].p[2] = Point(-sx, -sy,  sz);

        obj->shape.push_back(EnvelopeCollide::Mesh());
        obj->shape[5].r = r;
        obj->shape[5].type = EnvelopeCollide::TRIANGLE;
        obj->shape[5].p[0] = Point( sx, -sy,  sz);
        obj->shape[5].p[1] = Point( sx, -sy, -sz);
        obj->shape[5].p[2] = Point(-sx, -sy,  sz);

        obj->shape.push_back(EnvelopeCollide::Mesh());
        obj->shape[6].r = r;
        obj->shape[6].type = EnvelopeCollide::TRIANGLE;
        obj->shape[6].p[0] = Point(-sx,  sy, -sz);
        obj->shape[6].p[1] = Point( sx,  sy, -sz);
        obj->shape[6].p[2] = Point(-sx,  sy,  sz);

        obj->shape.push_back(EnvelopeCollide::Mesh());
        obj->shape[7].r = r;
        obj->shape[7].type = EnvelopeCollide::TRIANGLE;
        obj->shape[7].p[0] = Point( sx,  sy,  sz);
        obj->shape[7].p[1] = Point( sx,  sy, -sz);
        obj->shape[7].p[2] = Point(-sx,  sy,  sz);


        obj->shape.push_back(EnvelopeCollide::Mesh());
        obj->shape[8].r = r;
        obj->shape[8].type = EnvelopeCollide::TRIANGLE;
        obj->shape[8].p[0] = Point(-sx, -sy, -sz);
        obj->shape[8].p[1] = Point(-sx,  sy, -sz);
        obj->shape[8].p[2] = Point(-sx, -sy,  sz);

        obj->shape.push_back(EnvelopeCollide::Mesh());
        obj->shape[9].r = r;
        obj->shape[9].type = EnvelopeCollide::TRIANGLE;
        obj->shape[9].p[0] = Point(-sx,  sy,  sz);
        obj->shape[9].p[1] = Point(-sx,  sy, -sz);
        obj->shape[9].p[2] = Point(-sx, -sy,  sz);

        obj->shape.push_back(EnvelopeCollide::Mesh());
        obj->shape[10].r = r;
        obj->shape[10].type = EnvelopeCollide::TRIANGLE;
        obj->shape[10].p[0] = Point( sx, -sy, -sz);
        obj->shape[10].p[1] = Point( sx,  sy, -sz);
        obj->shape[10].p[2] = Point( sx, -sy,  sz);

        obj->shape.push_back(EnvelopeCollide::Mesh());
        obj->shape[11].r = r;
        obj->shape[11].type = EnvelopeCollide::TRIANGLE;
        obj->shape[11].p[0] = Point( sx,  sy,  sz);
        obj->shape[11].p[1] = Point( sx,  sy, -sz);
        obj->shape[11].p[2] = Point( sx, -sy,  sz);

        m->add(obj);
        ++licz;
    }

    /// Initializes octree, which will hold module objects.
    en.tree.prepare(4, Punkt(-40.0f, -120.0f, -40.0f), Punkt(40.0f, 260.0f, 40.0f));
    en.tree.addOnTilesetInitialize(m);
}
