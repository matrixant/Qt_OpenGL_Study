#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H
#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

class OpenGLWindow : public QOpenGLWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    OpenGLWindow();
    ~OpenGLWindow();
    QSize sizeHint() const;

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    QMatrix4x4 projMat;
    QMatrix4x4 viewMat;
    QMatrix4x4 modelMat;
    QVector<QVector3D> vertices;
    QVector<QVector3D> posCoords;
    QVector<QVector3D> colors;
    QOpenGLBuffer mVBO;
    QOpenGLVertexArrayObject mVAO;
    QOpenGLShaderProgram mShader;

signals:

public slots:
};

#endif // OPENGLWINDOW_H
