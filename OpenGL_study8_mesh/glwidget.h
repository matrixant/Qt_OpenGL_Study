#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <Qt3DRender/QCamera>
#include "mymesh.h"
#include <QKeyEvent>

struct Material {
    float specShineness;
};

struct PointLight {
    QVector3D position;
//    QVector3D color;
    QVector3D ambient;
    QVector3D diffuse;
    QVector3D specular;
    float attenuatConstant;
    float attenuatLinear;
    float attenuatQuadratic;
};

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLWidget(QOpenGLWidget *parent = 0);
    ~GLWidget();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void updateGL();

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    bool keyState[256];

    QOpenGLShaderProgram *myShaderProg;
    QMatrix4x4 modelMatrix;
    QOpenGLTexture *texTmp;

    Qt3DRender::QCamera myCamera;

    PointLight pointLight;

    QVector<Vertex> vertexes;
    QVector<quint32> indices;
    QVector<Texture> texture;

    MyMesh *mesh;

    void processKey();
};

#endif // GLWIDGET_H
