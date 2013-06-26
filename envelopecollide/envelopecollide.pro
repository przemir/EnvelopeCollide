QT       -= opengl
QT       -= gui core

TARGET = envelopecollide
TEMPLATE = lib
CONFIG += staticlib

SOURCES += Geometry/*.cpp CollisionSystem/*.cpp \
    CollisionSystem.cpp \
    Collision.cpp
HEADERS += Geometry/*.h CollisionSystem/*.h \
    CollisionSystem.h \
    Collision.h \
    CollisionSystem/ProjectorMoveVectorFilter.h \
    CollisionSystem/OctreeStructure.h \
    CollisionSystem/FilterFunction.h \
    CollisionSystem/BoundingSphereFilter.h \
    CollisionSystem/ProjectorSimpleFilter.h \
    CollisionSystem/BoundingSphereSimpleFilter.h \
    CollisionSystem/CollisionFilter.h \
    CollisionSystem/NoFilter.h

LIBS += -lGLU

INCLUDEPATH += .
DESTDIR = bin
