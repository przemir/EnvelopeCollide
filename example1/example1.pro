#-------------------------------------------------
#
# Project created by QtCreator 2013-02-09T15:06:34
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = example1
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
    Tree/Tree4Manager.cpp \
    Tree/SolidCriteriaGame.cpp \
    Tree/GameMeshExecutor.cpp \
    Situation.cpp \
    GuiSettings.cpp \
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
    Tree/Tree4Manager.h \
    Tree/SolidGame.h \
    Tree/GameMeshGenerator.h \
    Tree/GameMeshExecutor.h \
    Tree/SolidCriteriaGame.h \
    Tree/ResultCollectorAllQtTemplate.h \
    Tree/ResultCollectorCatcherQtTemplate.h \
    Situation.h \
    GuiSettings.h \
    Color.h

FORMS    += MainWindow.ui

#DESTDIR = bin
