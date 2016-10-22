#-------------------------------------------------
#
# Project created by QtCreator 2016-08-11T15:37:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenGL_study2_cube
TEMPLATE = app


SOURCES += main.cpp\
        openglwidget.cpp

HEADERS  += openglwidget.h

RESOURCES += \
    shaders.qrc

DISTFILES += \
    shaders/cube.vert \
    shaders/cube.frag
