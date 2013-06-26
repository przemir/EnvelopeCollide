/* 
 * File:   GLView.h
 * Author: Przemek
 *
 * Created on 1 październik 2011, 11:39
 */

#ifndef GLVIEW_H
#define	GLVIEW_H

#include <qgl.h>
#include <QTime>
#include <QTimer>
#include "Camera.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include "Module.h"
#include "Engine.h"
#include "Player.h"
#include<time.h>

class MainWindow;

class GLView : public QGLWidget
{
    Q_OBJECT
public:
    GLView(QWidget* parent = NULL);
    ~GLView();

    /// Sets pointer to main window widget.
    void setMainWindow(MainWindow * m);

    /// Handles keyboard events.
    void processKey(float elapsed_time);

protected:
    /// Creates openGL context.
    virtual void initializeGL();

    /// Rendering.
    virtual void paintGL();

    /// Widget resizing.
    virtual void resizeGL(int w, int h);

    /// Handling pressing key events.
    virtual void keyPressEvent(QKeyEvent* e);

    /// Handling releasing key events.
    virtual void keyReleaseEvent(QKeyEvent* e);

    /// Handling pressing mouse buttons events.
    virtual void mousePressEvent(QMouseEvent* e);

    /// Handling moving mouse buttons events.
    virtual void mouseMoveEvent(QMouseEvent* e);

    /// Handling releasing mouse buttons events.
    virtual void mouseReleaseEvent(QMouseEvent* e);

public slots:
    /// One second passed.
    void secondTimeOut();

private:
    /// Timers

    /// Timer showing how much time has elapsed since the last 'updateGL' procedure call.
    QTime elapsedTimer;

    /// Time after starting the application.
    float globalTime;

    /// Timer for repainting scene.
    QTimer* glTimer;

    /// Timer measuring FPS every second.
    QTimer* timerEverySecond;

    /// Current second ticks.
    int thisSecondTicks;

    /// FPS (frames per second) - 'glTimer' adjusts to this value
    int tick_rate;

    MainWindow* mainWindow;

    /// Is openGL initialized.
    bool contextGlInitialized;
};

#endif	/* GLVIEW_H */

