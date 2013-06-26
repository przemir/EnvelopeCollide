#-------------------------------------------------
#
# Project created by QtCreator 2013-02-09T15:06:34
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = example2
TEMPLATE = app
LIBS += -L../envelopecollide/bin/ -lenvelopecollide

 win32 {
     LIBS += -lGLU32
 }
 unix {
     LIBS += -lGLU
 }

INCLUDEPATH += ../envelopecollide

SOURCES += main.cpp\
        MainWindow.cpp \
    Object.cpp \
    Model.cpp \
    Module.cpp \
    Player.cpp \
    ModuleCreation.cpp \
    GLView.cpp \
    Camera.cpp \
    Engine.cpp \
    Controllers.cpp \
    Tree/Tree4Manager.cpp \
    ObjectSlowing.cpp \
    ObjectShowing.cpp \
    Moves.cpp \
    Tree/SolidCriteriaGame.cpp \
    Tree/GameMeshExecutor.cpp \
    Tree/GameMeshDynamicGenerator.cpp \
    Color.cpp

HEADERS  += MainWindow.h \
    Object.h \
    Model.h \
    Module.h \
    Player.h \
    ModuleCreation.h \
    GLView.h \
    Camera.h \
    Engine.h \
    Controllers.h \
    Tree/Tree4Manager.h \
    ObjectSlowing.h \
    ObjectShowing.h \
    Moves.h \
    Tree/ResultCollectorCatcherQtTemplate.h \
    Tree/ResultCollectorAllQtTemplate.h \
    Tree/SolidGame.h \
    Tree/SolidCriteriaGame.h \
    Tree/GameMeshGenerator.h \
    Tree/GameMeshExecutor.h \
    Tree/GameMeshDynamicGenerator.h \
    Color.h

FORMS    += MainWindow.ui

#DESTDIR = bin
