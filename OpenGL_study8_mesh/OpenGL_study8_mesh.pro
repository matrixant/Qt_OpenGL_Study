#-------------------------------------------------
#
# Project created by QtCreator 2016-08-30T20:21:05
#
#-------------------------------------------------

QT       += core gui 3dcore 3drender

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenGL_study8_mesh
TEMPLATE = app


SOURCES += main.cpp\
        glwidget.cpp \
    mymesh.cpp

HEADERS  += glwidget.h \
    mymesh.h

RESOURCES += resources.qrc
