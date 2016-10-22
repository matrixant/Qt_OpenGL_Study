#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QOpenGLTexture>

struct Material {
    QVector3D diffuse;
    QVector3D specular;
    float specShininess;
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

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    QMatrix4x4 projMatrix;
    QMatrix4x4 viewMatrix;
    QMatrix4x4 modelMatrix;

    QVector3D cameraPos;
    QVector3D viewCenter;
    QVector2D mousePressPos;
    QVector3D myUp;
    QVector3D myDown;
    QVector3D myLeft;
    QVector3D myRight;
    QVector3D myForward;
    QVector3D myBackward;
    bool keys[256];
    float moveSpeed;
    QPointF prePos;

    QVector<QVector3D> vertexes;
    QVector<QVector3D> normals;
    QVector<QVector2D> texCoords;
    QOpenGLBuffer *myVBO;
    QOpenGLVertexArrayObject *myVAO;
    QOpenGLShaderProgram *myShader;

    void myUpdate();

    PointLight pointLight;

    QOpenGLTexture *texture_diff;
    QOpenGLTexture *texture_spec;
};

#endif // GLWIDGET_H
