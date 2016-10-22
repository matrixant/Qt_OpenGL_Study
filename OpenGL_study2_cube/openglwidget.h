#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QTimer>
#include <QPushButton>
#include <QVBoxLayout>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
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
//    QTimer mTimer;
    QMatrix4x4 projMatrix;
    QMatrix4x4 modelMatrix;
    QMatrix4x4 viewMatrix;
    QVector<QVector3D> vertexes;
    QOpenGLBuffer mVBO;
    QOpenGLVertexArrayObject mVAO;
    QOpenGLShaderProgram mShader;
    QVector3D viewPos;

private slots:
    void mUpdate();
};

#endif // OPENGLWIDGET_H
