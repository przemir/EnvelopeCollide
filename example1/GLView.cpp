/* 
 * File:   GLView.cpp
 * Author: Przemysław Bągard
 * 
 * Created on 1 październik 2011, 11:39
 */

#include "GLView.h"
#include <QDebug>
#include <QtCore>
#include "Geometry/opengl.h"
#include "Geometry/shapes.h"
#include "ModuleCreation.h"
#include "Tree/ResultCollectorAllQtTemplate.h"
#include "Tree/ResultCollectorCatcherQtTemplate.h"
#include "MainWindow.h"
#include <GL/glu.h>

extern Engine& en;

GLView::GLView(QWidget* parent) : QGLWidget(parent), cameraRadSpeed(2.5f), cameraVecSpeed(1.0f), objectRadSpeed(5.0f)
{
    tick_rate = 50;
    globalTime = 0.0f;

    glTimer = new QTimer(this);
    connect(glTimer, SIGNAL(timeout()), this, SLOT(updateGL()));
    glTimer->start(1000/tick_rate);

    this->setFocusPolicy(Qt::ClickFocus);
    this->setFocus();
}

GLView::~GLView()
{
}

void GLView::setMainWindow(MainWindow *m)
{
    this->mainWindow = m;
}

void GLView::calculateNextMove()
{
    EnvelopeCollide::Entry entry;
    EnvelopeCollide::Mesh data = en.pl->getCurrentShape(0);
    entry.makeFromAtom(data);
    entry.vec = en.pl->front();
    entry.front_max = en.pl->vectorFrontLength;
    entry.back_max = en.pl->vectorBackLength;

    SolidCriteriaGame<SolidGame> crit;
    crit.type = (SolidCriteriaGame<SolidGame>::TYPE)en.getGui().criteriaMode;

    switch(en.getGui().logMode)
    {
    case 0: /// all collision events (object enters/exits/is inside another object).
    {
        ResultCollectorAllQtTemplate<SolidGame> res;
        en.tree.getInterfacesDetection(res, entry, crit, en.pl->id);
        en.resAll = res;
    }
    break;
    case 1: /// earliest collision moving forward, earliest collision going back.
    {
        ResultCollectorCatcherQtTemplate<SolidGame> res;
        en.tree.getInterfacesDetection(res, entry, crit, en.pl->id);
        en.resFirst = res;
    }
    break;
    default:
        break;
    }

    mainWindow->generateCollisionTree();
}


void GLView::initializeGL()
{
    /// openGL settings.
    glClearDepth(1.f);
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.f, 1.f, 1.f, 500.f);

    /// Crates project world.
    ModuleCreation::create(&en.module);
    en.module.initializeModule();

    /// Distinguished object - character.
    en.pl = dynamic_cast<Player*>(en.module.get(OBJECT_PLAYER));
    Q_ASSERT(en.pl != NULL);

    /// Sets character's properties and animation.
    en.pl->model.setAnimationState(0);
    elapsedTimer.start();

    /// Inits camera settings.
    en.camera.initialize(Punkt(-2.0f, 2.5f, 0.0f));

    /// openGL initialization is finished.
    emit glInitialized();
}


void GLView::paintGL()
{
    /// If mouse hovers collision result list, get data of hovered item.
    /// Player position may change thanks to moment of collision.
    QTreeWidgetItem* item = mainWindow->getTreeItemId();
    QPair<float, Object::Id> data = mainWindow->getItemData();
    if(item == NULL)
        en.pl->displayAtVecLength = 0.0f;
    else
        en.pl->displayAtVecLength = data.first;

    /// Time settings.
    unsigned long int miliseconds = elapsedTimer.elapsed();
    float elapsed_time = ((float)miliseconds)/1000.0f;
    elapsedTimer.restart();
    globalTime += elapsed_time;

    /// openGL settings.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0,0,width(),height());

    /// Camera settings.
    gluPerspective(45.0f,(GLfloat)width()/(GLfloat)height(),0.1f,100.0f);
    en.camera.setCamera();
    glMatrixMode(GL_MODELVIEW);			// Select The Modelview Matrix
    glLoadIdentity();					// Reset The Modelview Matrix

    /// Elapsed time.
    en.module.processTime(elapsed_time);

    /// Renders the module.
    glEnable(GL_TEXTURE_2D);
    en.module.render();

    /// Renders the ground.
    PKT rectA = en.tree.getTreeRangeMin();
    rectA.y = en.pl->pos.y;
    PKT rectB = en.tree.getTreeRangeMax();
    rectB.y = en.pl->pos.y;

    glDisable(GL_TEXTURE_2D);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    if(en.getGui().showPlane)
    {
        glBegin(GL_QUADS);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        renderVertex(Punkt(rectA.x,en.pl->pos.y,rectA.z), fPunkt(0,0));
        glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
        renderVertex(Punkt(rectB.x,en.pl->pos.y,rectA.z), fPunkt(0,1));
        glColor4f(0.75f, 0.75f, 0.75f, 1.0f);
        renderVertex(Punkt(rectB.x,en.pl->pos.y,rectB.z), fPunkt(1,1));
        glColor4f(0.25f, 0.25f, 0.25f, 1.0f);
        renderVertex(Punkt(rectA.x,en.pl->pos.y,rectB.z), fPunkt(1,0));
        glEnd();
    }

    glLineWidth(1.0f);
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
    glLineWidth(1.0f);
}


void GLView::resizeGL(int width, int height)
{
    if (height == 0)
        height = 1;

    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLView::keyPressEvent(QKeyEvent* e)
{
    /// Camera rotation.
    if(e->key() == Qt::Key_Q)
        en.camera.vAngle.x -= cameraRadSpeed;
    if(e->key() == Qt::Key_E)
        en.camera.vAngle.x += cameraRadSpeed;

    /// Camera movement (forward, back, left, right)
    if(e->key() == Qt::Key_W)
    {
        en.camera.pos.x = float(en.camera.pos.x + cameraVecSpeed*cos(degToRad(en.camera.vAngle.x)));
        en.camera.pos.z = float(en.camera.pos.z + cameraVecSpeed*sin(degToRad(en.camera.vAngle.x)));
    }
    if(e->key() == Qt::Key_S)
    {
        en.camera.pos.x = float(en.camera.pos.x - cameraVecSpeed*cos(degToRad(en.camera.vAngle.x)));
        en.camera.pos.z = float(en.camera.pos.z - cameraVecSpeed*sin(degToRad(en.camera.vAngle.x)));
    }
    if(e->key() == Qt::Key_A)
    {
        en.camera.pos.x = float(en.camera.pos.x + cameraVecSpeed*sin(degToRad(en.camera.vAngle.x)));
        en.camera.pos.z = float(en.camera.pos.z - cameraVecSpeed*cos(degToRad(en.camera.vAngle.x)));
    }
    if(e->key() == Qt::Key_D)
    {
        en.camera.pos.x = float(en.camera.pos.x - cameraVecSpeed*sin(degToRad(en.camera.vAngle.x)));
        en.camera.pos.z = float(en.camera.pos.z + cameraVecSpeed*cos(degToRad(en.camera.vAngle.x)));
    }

    /// Character rotation.
    if(e->key() == Qt::Key_Left)
    {
        en.pl->radXZ += objectRadSpeed;
        if(en.pl->radXZ >= 360.0f)
            en.pl->radXZ -= 360.0f;
        emit playerPosChanged();
    }
    if(e->key() == Qt::Key_Right)
    {
        en.pl->radXZ -= objectRadSpeed;
        if(en.pl->radXZ < 0.0f)
            en.pl->radXZ += 360.0f;
        emit playerPosChanged();
    }

    /// Character movement.
    bool changePos = false;
    PKT changeVec = en.pl->front();
    if(e->key() == Qt::Key_Up)
        changePos = true;
    if(e->key() == Qt::Key_Down)
    {
        changePos = true;
        changeVec = -changeVec;
    }
    if(changePos)
    {
        en.pl->pos += changeVec;
        emit playerPosChanged();
    }

    e->accept();
}

void GLView::keyReleaseEvent(QKeyEvent* e)
{
}

void GLView::mousePressEvent(QMouseEvent* e)
{
}

void GLView::mouseMoveEvent(QMouseEvent* e)
{
}

void GLView::mouseReleaseEvent(QMouseEvent* e)
{
}
