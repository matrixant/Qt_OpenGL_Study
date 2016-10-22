#include "openglwindow.h"
#include <QDebug>

OpenGLWindow::OpenGLWindow()
    :  QOpenGLWindow(){

}

OpenGLWindow::~OpenGLWindow() {
    mShader.removeAllShaders();
    mShader.deleteLater();
    mVBO.destroy();
    mVAO.destroy();
}

QSize OpenGLWindow::sizeHint() const {
    return QSize(640, 480);
}

void OpenGLWindow::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_CULL_FACE);
    glClearColor(0, 0, 0, 1);

//    modelMat.setToIdentity();
//    modelMat.rotate(90, QVector3D(0, 1, 0));
//    viewMat.lookAt(QVector3D(0, 0, -4), QVector3D(0, 0, 0), QVector3D(0, 1, 0));

    mVAO.create();
    mVAO.bind();
    //verts and colors
    vertices << QVector3D(1, -1, 1) << QVector3D(1, 0, 0)
             << QVector3D(0, 1, 1) << QVector3D(0, 1, 0)
             << QVector3D(-1, -1, 1) << QVector3D(0, 0, 1);

    posCoords << QVector3D(1, -1, 1) << QVector3D(0, 1, 1) << QVector3D(-1, -1, 1);
    colors << QVector3D(1, 0, 0) << QVector3D(0, 1, 0) << QVector3D(0, 0, 1);

    mVBO.create();
    mVBO.bind();
    mVBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    mVBO.allocate(vertices.constData(), vertices.size() * sizeof(QVector3D));

    qDebug() << "vertices size is:" << vertices.size() << "Vector3D size is:" << sizeof(QVector3D);

    mShader.create();
    mShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vert/test.vert");
    mShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/frag/test.frag");
    mShader.link();
    mShader.bind();

    qDebug() << "VBO size is:" << mVBO.size();

    mShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 2 * sizeof(QVector3D));
    mShader.enableAttributeArray(0);
    mShader.setAttributeBuffer(1, GL_FLOAT, sizeof(QVector3D), 3, 2 * sizeof(QVector3D));
    mShader.enableAttributeArray(1);

    mVBO.release();
    mVAO.release();
    mShader.release();
}

void OpenGLWindow::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    mShader.bind();
    mVAO.bind();

//    mShader.setUniformValue("mvpMat", projMat * viewMat * modelMat);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 2);

    mVAO.release();
    mShader.release();
}

void OpenGLWindow::resizeGL(GLint width, GLint height) {
    if(height == 0) height = 1;
    if(width == 0) width = 1;

//    projMat.setToIdentity();
//    projMat.perspective(45.0f, width / float(height), 0.1f, 100.0f);
    glViewport(0, 0, width, height);
}
