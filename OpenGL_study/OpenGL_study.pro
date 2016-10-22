#-------------------------------------------------
#
# Project created by QtCreator 2016-08-02T22:26:14
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenGL_study
TEMPLATE = app


SOURCES += main.cpp\
    openglwindow.cpp

HEADERS  += \
    openglwindow.h \
    vertex.h

DISTFILES += \
    shaders/test.vert \
    test.frag

RESOURCES += \
    shaders.qrc
