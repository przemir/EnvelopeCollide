TEMPLATE = app
CONFIG += console
CONFIG -= qt

LIBS += -L../envelopecollide/bin/ -lenvelopecollide

INCLUDEPATH += ../envelopecollide

SOURCES += main.cpp

HEADERS += \
    tests.h \
    generator.h

