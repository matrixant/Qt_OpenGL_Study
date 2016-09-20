#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QMouseEvent>

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

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
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
    QOpenGLBuffer *myVBO;
    QOpenGLVertexArrayObject *myVAO;
    QOpenGLShaderProgram *myShader;

    void myUpdate();
};

#endif // GLWIDGET_H
