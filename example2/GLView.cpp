/*
 * File:   GLView.cpp
 * Author: Przemek
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
#include "Tree/SolidCriteriaGame.h"
#include "MainWindow.h"
#include "QMessageBox"
#include "QStatusBar"
#include <QFileDialog>
#include <GL/glu.h>

using namespace EnvelopeGeometry;
extern Engine& en;

GLView::GLView(QWidget* parent) : QGLWidget(parent)
{
    tick_rate = 50;
    globalTime = 0.0f;
    thisSecondTicks = 0;
    glTimer = new QTimer(this);
    timerEverySecond = new QTimer(this);

    /// connections - timer timeout
    connect(glTimer, SIGNAL(timeout()), this, SLOT(updateGL()));
    connect(timerEverySecond, SIGNAL(timeout()), this, SLOT(secondTimeOut()));

    this->setFocusPolicy(Qt::ClickFocus);
    this->setFocus();

    /// Initializes timer measuring FPS every second.
    glTimer->start(1000/tick_rate);

    /// Initializes sequence of moves
    en.moves = new Moves(this);
    en.moves->mode = Moves::MODE_NONE;

    contextGlInitialized = false;
}

GLView::~GLView()
{
    delete en.moves;
    en.moves = NULL;
}

void GLView::setMainWindow(MainWindow *m)
{
    this->mainWindow = m;
}

void GLView::initializeGL()
{
    if(contextGlInitialized)
        return;

    contextGlInitialized = true;

    /// openGL settings
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
    en.pl->model.setAnimationState(ANIMATION_STAND);

    /// Sets timer for repainting scene.
    elapsedTimer.start();

    /// Inits camera settings.
    en.camera.initialize(Punkt(-2.0f, 2.5f, 0.0f));
    en.camera.mode = Camera::NORMAL;

    /// Starts timer measuring FPS.
    timerEverySecond->start(1000);
}

void GLView::paintGL()
{
    /// Time settings.
    unsigned long int miliseconds = elapsedTimer.elapsed();
    float elapsed_time = ((float)miliseconds)/1000.0f;
    elapsedTimer.restart();
    globalTime += elapsed_time;
    ++thisSecondTicks;

    /// In sequence mode elapsed_time is always the same (to make played sequence more deterministic).
    if(en.moves->mode == Moves::MODE_PLAY)
        elapsed_time = 0.02f;

    /// Handles keys.
    processKey(elapsed_time);

    /// openGL
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0,0,width(),height());
    gluPerspective(45.0f,(GLfloat)width()/(GLfloat)height(),0.1f,100.0f);
    en.camera.setCamera();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

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

    glBegin(GL_QUADS);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    renderVertex(Punkt(rectA.x,0.0f,rectA.z), fPunkt(0,0));
    glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
    renderVertex(Punkt(rectB.x,0.0f,rectA.z), fPunkt(0,1));
    glColor4f(0.75f, 0.75f, 0.75f, 1.0f);
    renderVertex(Punkt(rectB.x,0.0f,rectB.z), fPunkt(1,1));
    glColor4f(0.25f, 0.25f, 0.25f, 1.0f);
    renderVertex(Punkt(rectA.x,0.0f,rectB.z), fPunkt(1,0));
    glEnd();

    glLineWidth(1.0f);
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
    glLineWidth(1.0f);

    /// Removes pressed/released keys in this frame.
    en.control.clearTurn();
}

void GLView::processKey(float elapsed_time)
{
    if(en.moves->mode != Moves::MODE_PLAY)
    {
        /// Controls mode.

        /// Character rotation.
        float speedViewAngle = 60;
        if(en.control.keyIsPressed(en.control.keyRotateLeft))
            en.pl->radXZ += speedViewAngle*elapsed_time;
        if(en.control.keyIsPressed(en.control.keyRotateRight))
            en.pl->radXZ -= speedViewAngle*elapsed_time;

        /// Character movement.
        float elapsed_time_with_direction = 0.0f;
        if(en.control.keyIsPressed(en.control.keyUp))
            elapsed_time_with_direction = elapsed_time;
        if(en.control.keyIsPressed(en.control.keyDown))
            elapsed_time_with_direction = -elapsed_time;
        if(en.control.keyIsPressed(en.control.keyUp) == en.control.keyIsPressed(en.control.keyDown))
            elapsed_time_with_direction = 0.0f;

        /// Sets animation state depending on whether character moves or not.
        if(elapsed_time_with_direction == 0.0f)
        {
            if(en.pl->model.getCurrentState() != ANIMATION_STAND)
                en.pl->model.setAnimationState(ANIMATION_STAND);
        }
        else
        {
            if(en.pl->model.getCurrentState() != ANIMATION_MOVE)
                en.pl->model.setAnimationState(ANIMATION_MOVE);
        }

        /// Checks collision.
        en.processMove(elapsed_time_with_direction);
    }
    else /// Play sequence mode
        en.moves->playAnimation(elapsed_time);
}

void GLView::resizeGL(int width, int height)
{
    if (height==0)
        height=1;
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLView::secondTimeOut()
{
    /// Display FPS.
    QString sfps;
    sfps = QString("FPS: %1 mode: ").arg(thisSecondTicks);

    /// Display program mode (control/play sequence)
    switch(en.moves->mode)
    {
    case Moves::MODE_NONE: sfps += "control"; break;
    case Moves::MODE_PLAY: sfps += "play sequence"; break;
    default:
        break;
    }
    mainWindow->statusBar()->showMessage(sfps);

    /// Resets FPS count.
    thisSecondTicks = 0;
}


void GLView::keyPressEvent(QKeyEvent* e)
{
    en.control.setKeyPressed(e->key());

    if(e->key() == Qt::Key_F1)
    {
        /// Shows controls information.
        QMessageBox::about(this, tr("CollisionExample"), tr("Controls\n\nUp/Down arrow key - go forward/back\nLeft/Right arrow key - rotation\nL - load sequence from file\nP - display sequence"));
    }
    else if(e->key() == Qt::Key_F2)
    {
        /// Shows information about author.
        QMessageBox::about(this, tr("CollisionExample"), tr("Collision system demo.\nAuthor: Przemysław Bągard"));
    }
    else if(e->key() == Qt::Key_F3)
    {
        /// Shows information about Qt.
        qApp->aboutQt();
    }

    if(e->key() == Qt::Key_P)
    {
        if(en.moves->mode == Moves::MODE_NONE)
        {
            /// Enables playing sequence of moves.
            en.moves->firstMove();
            //en.moves->applyNext();
            en.moves->mode = Moves::MODE_PLAY;
        }
        else if(en.moves->mode == Moves::MODE_PLAY)
        {
            /// Disables playing sequence of moves.
            en.moves->mode = Moves::MODE_NONE;
        }
    }

    if(e->key() == Qt::Key_L)
    {
        /// Loads sequence of moves from file.
        QString filename = QFileDialog::getOpenFileName(this, "Load animation file format");
        if(filename != "")
        {
            if(en.moves->loadFromFile(filename))
            {
                if(en.moves->mode == Moves::MODE_PLAY)
                    en.moves->firstMove();
                else
                    en.moves->mode = Moves::MODE_NONE;
            }
            else
            {
                /// Sequence of moves cannot be loaded.
                en.moves->mode = Moves::MODE_NONE;
                en.processMove(0.0f);
            }
        }
    }

    e->accept();
    return;
}

void GLView::keyReleaseEvent(QKeyEvent* e)
{
    en.control.setKeyUnPressed(e->key());
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
