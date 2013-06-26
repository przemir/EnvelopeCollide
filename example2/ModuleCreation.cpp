#include "ModuleCreation.h"
#include "Module.h"
#include "ModelData.h"      /// character model
#include "ModelData2.h"     /// box model
#include "QImage"
#include "Engine.h"
#include "Player.h"
#include <QDebug>
#include "ObjectSlowing.h"
#include "ObjectShowing.h"
#include "Color.h"

extern Engine& en;


ModuleCreation::ModuleCreation()
{
}

void ModuleCreation::create(Module *m)
{
    {
        /// Character.

        Object* obj = new Player();

        /// Loads model and animation.
        obj->model.setModel(model_vertex, model_texCoord, model_tri, 1144, 404);
        obj->model.setAnimation(animation_frame_start, 6, 0.25f);
        obj->hardShapeColor = Color(0.0f, 0.0f, 1.0f);
        obj->hardShape.push_back(EnvelopeCollide::Mesh());
        obj->hardShape[0].r = 1.0f;
        obj->hardShape[0].type = EnvelopeCollide::LINE;
        obj->hardShape[0].p[0] = Point(0, 1.0, 0);
        obj->hardShape[0].p[1] = Point(0, 3.25, 0);

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
        /// box

        Object* obj = new Object();
        obj->id = licz;
        obj->model.setModel(model_box_vertex, model_box_texCoord, model_box_tri, 8, 12);
        obj->pos = Point(6, 0, 3);

        /// Loads texture from file.
        QImage image;
        if(!image.load("tex2.bmp"))
        {
            qDebug() << "Texture file cannot be loaded: " << "tex2.bmp";
            qDebug() << image.text();
        }
        obj->model.createTexture(image);

        /// Adds object to module.
        m->add(obj);

        /// Next object will have next index.
        ++licz;
    }
    {
        /// 'hard' shape

        Object* obj = new Object();
        obj->id = licz;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(8, 0, 8);
        obj->hardShape.push_back(EnvelopeCollide::Mesh());
        obj->hardShape[0].r = 1.0f;
        obj->hardShape[0].type = EnvelopeCollide::LINE;
        obj->hardShape[0].p[0] = Point(0, 1, 0);
        obj->hardShape[0].p[1] = Point(0, 20, 0);

        /// Adds object to module.
        m->add(obj);
        ++licz;
    }
    {
        /// 'soft' shape - slowns down the character

        Object* obj = new ObjectSlowing();
        obj->id = licz;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(12, 0, 8);
        obj->softShapeColor = Color(0.0f, 1.0f, 0.0f);
        obj->softShape.push_back(EnvelopeCollide::Mesh());
        obj->softShape[0].r = 1.0f;
        obj->softShape[0].type = EnvelopeCollide::LINE;
        obj->softShape[0].p[0] = Point(0, 0, 0);
        obj->softShape[0].p[1] = Point(0, 10, 0);

        m->add(obj);
        ++licz;
    }

    {
        /// 'soft' shape - slowns down the character

        Object* obj = new ObjectSlowing();
        obj->id = licz;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(-16, 2, -12);
        obj->softShapeColor = Color(0.0f, 1.0f, 0.0f);
        obj->softShape.push_back(EnvelopeCollide::Mesh());
        obj->softShape[0].r = 1.0f;
        obj->softShape[0].type = EnvelopeCollide::POINT;
        obj->softShape[0].p[0] = Point(0, 0, 0);
        obj->softShape[0].p[1] = Point(0, 10, 0);

        m->add(obj);
        ++licz;
    }


    {
        /// 'soft' shape - shows another object when character enters this object.

        Object* obj = new ObjectShowing(OBJECT_HIDDEN_HARD_SHAPE);
        /// dynamic object (appear/dissapear)
        obj->dynamic = true;

        /// Object is hidden at the start.
        obj->exist = false;
        obj->id = OBJECT_HIDDEN_SHAPES;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(4, 0, 4);
        obj->softShapeColor = Color(0.0f, 0.0f, 1.0f);
        obj->softShape.push_back(EnvelopeCollide::Mesh());
        obj->softShape[0].r = 1.0f;
        obj->softShape[0].type = EnvelopeCollide::LINE;
        obj->softShape[0].p[0] = Point(0, 0, 0);
        obj->softShape[0].p[1] = Point(0, 10, 0);
        obj->hardShape.push_back(EnvelopeCollide::Mesh());
        obj->hardShape[0].r = 1.0f;
        obj->hardShape[0].type = EnvelopeCollide::LINE;
        obj->hardShape[0].p[0] = Point(0, 0, -2);
        obj->hardShape[0].p[1] = Point(0, 10, -2);

        m->add(obj);
    }

    {
        /// 'soft' shape - shows another object when character enters this object.
        Object* obj = new ObjectShowing(OBJECT_HIDDEN_SHAPES);

        /// dynamic object (appear/dissapear)
        obj->dynamic = true;
        obj->softShapeColor = Color(1.0f, 1.0f, 0.0f);
        obj->id = licz;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(-2, 0, 4);
        obj->softShape.push_back(EnvelopeCollide::Mesh());
        obj->softShape[0].r = 1.0f;
        obj->softShape[0].type = EnvelopeCollide::LINE;
        obj->softShape[0].p[0] = Point(0, 0, 0);
        obj->softShape[0].p[1] = Point(0, 10, 0);

        m->add(obj);
        ++licz;
    }

    {
        /// 'soft' shape - shows/hides another object when character enters/exits this object.
        Object* obj = new ObjectShowing(OBJECT_HIDDEN_SHAPES, true);
        obj->dynamic = true;
        obj->softShapeColor = Color(1.0f, 0.5f, 0.0f);
        obj->id = licz;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(-2, 0, -2);
        obj->softShape.push_back(EnvelopeCollide::Mesh());
        obj->softShape[0].r = 1.0f;
        obj->softShape[0].type = EnvelopeCollide::LINE;
        obj->softShape[0].p[0] = Point(0, 0, 0);
        obj->softShape[0].p[1] = Point(0, 10, 0);

        m->add(obj);
        ++licz;
    }

    {
        /// 'hard' shape

        Object* obj = new Object();
        obj->id = licz;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(12, 0, 16);
        obj->hardShape.push_back(EnvelopeCollide::Mesh());
        obj->hardShape[0].r = 0.75f;
        obj->hardShape[0].type = EnvelopeCollide::LINE;
        obj->hardShape[0].p[0] = Point(-2, 0, 0.3);
        obj->hardShape[0].p[1] = Point(5, 2, 0);

        m->add(obj);
        ++licz;
    }

    {
        Object* obj = new Object();
        obj->id = licz;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(20, 2, 6);
        obj->hardShape.push_back(EnvelopeCollide::Mesh());
        obj->hardShape[0].r = 0.75f;
        obj->hardShape[0].type = EnvelopeCollide::LINE;
        obj->hardShape[0].p[0] = Point(-2, 0, 2);
        obj->hardShape[0].p[1] = Point(2, 0, -1);

        m->add(obj);
        ++licz;
    }

    {
        Object* obj = new Object();
        obj->id = licz;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(8, 1, -4);
        obj->hardShape.push_back(EnvelopeCollide::Mesh());
        obj->hardShape[0].r = 0.75f;
        obj->hardShape[0].type = EnvelopeCollide::TRIANGLE;
        obj->hardShape[0].p[0] = Point(-4, 0, -4);
        obj->hardShape[0].p[1] = Point(0, 0, -5.2);
        obj->hardShape[0].p[2] = Point(-5, 0, -1.4);

        m->add(obj);
        ++licz;
    }

    {
        Object* obj = new Object();
        obj->id = licz;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(-3, 1, -8);
        obj->hardShape.push_back(EnvelopeCollide::Mesh());
        obj->hardShape[0].r = 0.75f;
        obj->hardShape[0].type = EnvelopeCollide::TRIANGLE;
        obj->hardShape[0].p[0] = Point(-4, -0.1, -4);
        obj->hardShape[0].p[1] = Point(0, 2.2, -3.8);
        obj->hardShape[0].p[2] = Point(-3.6, 0.6, -1.2);

        m->add(obj);
        ++licz;
    }

    {
        Object* obj = new Object();
        obj->id = licz;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(-9, 1, -12);
        obj->hardShape.push_back(EnvelopeCollide::Mesh());
        obj->hardShape[0].r = 1.0f;
        obj->hardShape[0].type = EnvelopeCollide::TRIANGLE;
        obj->hardShape[0].p[0] = Point(-3.2, -0.1, -4);
        obj->hardShape[0].p[1] = Point(0, 4.8, -3.0);
        obj->hardShape[0].p[2] = Point(-4, 0.6, -1.2);

        m->add(obj);
        ++licz;
    }

    {
        Object* obj = new Object();
        obj->id = licz;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(-10, 1, 1);
        obj->hardShape.push_back(EnvelopeCollide::Mesh());
        obj->hardShape[0].r = 1.0f;
        obj->hardShape[0].type = EnvelopeCollide::TRIANGLE;
        obj->hardShape[0].p[0] = Point(-3.2, -0.1, -4);
        obj->hardShape[0].p[1] = Point(0, 0.2, -3.0);
        obj->hardShape[0].p[2] = Point(-4, 0.6, -1.2);

        m->add(obj);
        ++licz;
    }
    {
        Object* obj = new Object();
        obj->dynamic = true;
        obj->exist = false;
        obj->id = OBJECT_HIDDEN_HARD_SHAPE;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(-10, 1, -4);
        obj->hardShape.push_back(EnvelopeCollide::Mesh());
        obj->hardShape[0].r = 1.0f;
        obj->hardShape[0].type = EnvelopeCollide::TRIANGLE;
        obj->hardShape[0].p[0] = Point(-4, -0.1, -4);
        obj->hardShape[0].p[1] = Point(0, 0.2, -3.0);
        obj->hardShape[0].p[2] = Point(-3.2, 0.6, -1.2);

        m->add(obj);
        ++licz;
    }

    {
        Object* obj = new Object();
        obj->dynamic = true;
        obj->exist = true;
        obj->id = licz;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(-15, 1, 15);
        obj->hardShape.push_back(EnvelopeCollide::Mesh());
        obj->hardShape[0].r = 1.0f;
        obj->hardShape[0].type = EnvelopeCollide::TRIANGLE;
        obj->hardShape[0].p[0] = Point(0, 0, 0);
        obj->hardShape[0].p[1] = Point(8, 0, 0);
        obj->hardShape[0].p[2] = Point(-8, 0, 4);

        m->add(obj);
        ++licz;
    }


    {
        Object* obj = new Object();
        obj->id = licz;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(20, 3, 20);
        obj->hardShape.push_back(EnvelopeCollide::Mesh());
        obj->hardShape[0].r = 0.2f;
        obj->hardShape[0].type = EnvelopeCollide::POINT;
        obj->hardShape[0].p[0] = Point(0, 0, 0);

        m->add(obj);
        ++licz;
    }

    {
        Object* obj = new Object();
        obj->id = licz;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(25, 2, 17);
        obj->hardShape.push_back(EnvelopeCollide::Mesh());
        obj->hardShape[0].r = 1.0f;
        obj->hardShape[0].type = EnvelopeCollide::POINT;
        obj->hardShape[0].p[0] = Point(0, 0, 0);

        m->add(obj);
        ++licz;
    }

    /// Walls closing world space.
    PKT rectMin = Point(-32, 6, -32);
    PKT rectMax = Point( 32,  -1,  32);
    licz = OBJECT_FIRST_WALL;
    float wallR = 0.01f;
    {
        Object* obj = new Object();
        obj->id = licz;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(0, 0, rectMin.z);
        obj->hardShape.push_back(EnvelopeCollide::Mesh());
        obj->hardShape[0].r = wallR;
        obj->hardShape[0].type = EnvelopeCollide::TRIANGLE;
        obj->hardShape[0].p[0] = Point(rectMin.x, rectMin.y, 0.0);
        obj->hardShape[0].p[1] = Point(rectMax.x, rectMin.y, 0.0);
        obj->hardShape[0].p[2] = Point(rectMin.x, rectMax.y, 0.0);
        obj->hardShape.push_back(EnvelopeCollide::Mesh());
        obj->hardShape[1].r = wallR;
        obj->hardShape[1].type = EnvelopeCollide::TRIANGLE;
        obj->hardShape[1].p[0] = Point(rectMax.x, rectMax.y, 0.0);
        obj->hardShape[1].p[1] = Point(rectMax.x, rectMin.y, 0.0);
        obj->hardShape[1].p[2] = Point(rectMin.x, rectMax.y, 0.0);

        m->add(obj);
        ++licz;
    }
    {
        Object* obj = new Object();
        obj->id = licz;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(0, 0, rectMax.z);
        obj->hardShape.push_back(EnvelopeCollide::Mesh());
        obj->hardShape[0].r = wallR;
        obj->hardShape[0].type = EnvelopeCollide::TRIANGLE;
        obj->hardShape[0].p[0] = Point(rectMin.x, rectMin.y, 0.0);
        obj->hardShape[0].p[1] = Point(rectMax.x, rectMin.y, 0.0);
        obj->hardShape[0].p[2] = Point(rectMin.x, rectMax.y, 0.0);
        obj->hardShape.push_back(EnvelopeCollide::Mesh());
        obj->hardShape[1].r = wallR;
        obj->hardShape[1].type = EnvelopeCollide::TRIANGLE;
        obj->hardShape[1].p[0] = Point(rectMax.x, rectMax.y, 0.0);
        obj->hardShape[1].p[1] = Point(rectMax.x, rectMin.y, 0.0);
        obj->hardShape[1].p[2] = Point(rectMin.x, rectMax.y, 0.0);

        m->add(obj);
        ++licz;
    }
    {
        Object* obj = new Object();
        obj->id = licz;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(rectMin.x, 0, 0);
        obj->hardShape.push_back(EnvelopeCollide::Mesh());
        obj->hardShape[0].r = wallR;
        obj->hardShape[0].type = EnvelopeCollide::TRIANGLE;
        obj->hardShape[0].p[0] = Point(0.0, rectMin.y, rectMin.z);
        obj->hardShape[0].p[1] = Point(0.0, rectMin.y, rectMax.z);
        obj->hardShape[0].p[2] = Point(0.0, rectMax.y, rectMin.z);
        obj->hardShape.push_back(EnvelopeCollide::Mesh());
        obj->hardShape[1].r = wallR;
        obj->hardShape[1].type = EnvelopeCollide::TRIANGLE;
        obj->hardShape[1].p[0] = Point(0.0, rectMax.y, rectMax.z);
        obj->hardShape[1].p[1] = Point(0.0, rectMin.y, rectMax.z);
        obj->hardShape[1].p[2] = Point(0.0, rectMax.y, rectMin.z);

        m->add(obj);
        ++licz;
    }
    {
        Object* obj = new Object();
        obj->id = licz;
        obj->model.setModel(0, 0, 0, 0, 0);
        obj->pos = Point(rectMax.x, 0, 0);
        obj->hardShape.push_back(EnvelopeCollide::Mesh());
        obj->hardShape[0].r = wallR;
        obj->hardShape[0].type = EnvelopeCollide::TRIANGLE;
        obj->hardShape[0].p[0] = Point(0.0, rectMin.y, rectMin.z);
        obj->hardShape[0].p[1] = Point(0.0, rectMin.y, rectMax.z);
        obj->hardShape[0].p[2] = Point(0.0, rectMax.y, rectMin.z);
        obj->hardShape.push_back(EnvelopeCollide::Mesh());
        obj->hardShape[1].r = wallR;
        obj->hardShape[1].type = EnvelopeCollide::TRIANGLE;
        obj->hardShape[1].p[0] = Point(0.0, rectMax.y, rectMax.z);
        obj->hardShape[1].p[1] = Point(0.0, rectMin.y, rectMax.z);
        obj->hardShape[1].p[2] = Point(0.0, rectMax.y, rectMin.z);

        m->add(obj);
        ++licz;
    }
    /// Walls - end

    /// Initializes octrees, which will hold module objects.
    en.tree.prepare(4, Point(-40.0f, -120.0f, -40.0f), Point(40.0f, 260.0f, 40.0f));
    en.tree.addOnTilesetInitialize(m);
}
