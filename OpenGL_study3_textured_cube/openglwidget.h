#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

class OpenGLWidget :
        public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    OpenGLWidget(QOpenGLWidget *parent = 0);
    ~OpenGLWidget();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    QVector<QVector3D> vertexes;
    QVector<QVector2D> texCoord;
    QMatrix4x4 modelMatrix;
    QMatrix4x4 viewMatrix;
    QMatrix4x4 projectionMatrix;
    QOpenGLBuffer *mVBO;
    QOpenGLVertexArrayObject mVAO;
    QOpenGLTexture *mTexture;
    QOpenGLShaderProgram mShader;
    GLuint positionLayout;
    GLuint colorLayout;
    GLuint textureLayout;

    void mUpdate();
};

#endif // OPENGLWIDGET_H
