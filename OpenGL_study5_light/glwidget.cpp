#include "glwidget.h"
#include <QtMath>
#include <QDebug>

GLWidget::GLWidget(QOpenGLWidget *parent) :
    QOpenGLWidget(parent) {
    QSurfaceFormat mySurface;
    mySurface.setRenderableType(QSurfaceFormat::OpenGL);
    mySurface.setProfile(QSurfaceFormat::CoreProfile);
    mySurface.setVersion(4, 0);

    this->setFormat(mySurface);

    connect(this, &GLWidget::frameSwapped, this, &GLWidget::myUpdate);

    memset(keys, 0, 256);
    moveSpeed = 0.01f;
    myUp = QVector3D(0, 1, 0);
    myDown = QVector3D(0, -1, 0);
    myLeft = QVector3D(1, 0, 0);
    myRight = QVector3D(-1, 0, 0);
    myForward = QVector3D(0, 0, 1);
    myBackward = QVector3D(0, 0, -1);

    lightColor = QVector3D(1.0f, 1.0f, 1.0f);
    lightPos = QVector3D(4, 4, 4);

    cursor().setPos(this->width() / 2, this->height() / 2);
    prePos = cursor().pos();

}

GLWidget::~GLWidget() {
    delete myVBO;
    delete myVAO;
    delete myShader;
}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_CULL_FACE);
    glClearColor(0, 0.2f, 0, 1);

    modelMatrix.setToIdentity();
    modelMatrix.rotate(20.0f, QVector3D(1, 0, 0));
    cameraPos.setZ(3.0f);
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(cameraPos, viewCenter, QVector3D(0, 1, 0));

    myVAO = new QOpenGLVertexArrayObject;
    myVAO->create();
    myVAO->bind();

    vertexes
            //front
            << QVector3D(-0.5f, -0.5f, 0.5f) << QVector3D(0.5f, -0.5f, 0.5f) << QVector3D(0.5f, 0.5f, 0.5f) << QVector3D(-0.5, 0.5f, 0.5f)
            //left
            << QVector3D(-0.5f, -0.5f, -0.5f) << QVector3D(-0.5f, -0.5f, 0.5f) << QVector3D(-0.5f, 0.5f, 0.5f) << QVector3D(-0.5f, 0.5f, -0.5f)
            //back
            << QVector3D(0.5f, -0.5f, -0.5f) << QVector3D(-0.5f, -0.5f, -0.5f) << QVector3D(-0.5f, 0.5f, -0.5f) << QVector3D(0.5f, 0.5f, -0.5f)
            //right
            << QVector3D(0.5f, -0.5f, 0.5f) << QVector3D(0.5f, -0.5f, -0.5f) << QVector3D(0.5f, 0.5f, -0.5f) << QVector3D(0.5f, 0.5f, 0.5f)
            //top
            << QVector3D(-0.5f, 0.5f, 0.5f) << QVector3D(0.5f, 0.5f, 0.5f) << QVector3D(0.5f, 0.5f, -0.5f) << QVector3D(-0.5f, 0.5f, -0.5f)
            //bottom
            << QVector3D(-0.5f, -0.5f, -0.5f) << QVector3D(0.5f, -0.5f, -0.5f) << QVector3D(0.5f, -0.5f, 0.5f) << QVector3D(-0.5f, -0.5f, 0.5f)
            ;
    int vertLen = vertexes.size() * sizeof(QVector3D);
    normals
            //front
            << QVector3D(0, 0, 1) << QVector3D(0, 0, 1) << QVector3D(0, 0, 1) << QVector3D(0, 0, 1)
            //left
            << QVector3D(-1, 0, 0) << QVector3D(-1, 0, 0) << QVector3D(-1, 0, 0) << QVector3D(-1, 0, 0)
            //back
            << QVector3D(0, 0, -1) << QVector3D(0, 0, -1) << QVector3D(0, 0, -1) << QVector3D(0, 0, -1)
            //right
            << QVector3D(1, 0, 0) << QVector3D(1, 0, 0) << QVector3D(1, 0, 0) << QVector3D(1, 0, 0)
            //top
            << QVector3D(0, 1, 0) << QVector3D(0, 1, 0) << QVector3D(0, 1, 0) << QVector3D(0, 1, 0)
            //bottom
            << QVector3D(0, -1, 0) << QVector3D(0, -1, 0) << QVector3D(0, -1, 0) << QVector3D(0, -1, 0)

            ;
    int normalLen = normals.size() * sizeof(QVector3D);

    myVBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    myVBO->create();
    myVBO->bind();
    myVBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
    myVBO->allocate(vertexes.constData(), vertLen + normalLen);
    myVBO->write(vertLen, normals.constData(), normalLen);

    myShader = new QOpenGLShaderProgram;
    myShader->create();
    myShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/cube.vert");
    myShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/cube.frag");
    myShader->link();
    myShader->bind();

    myShader->setAttributeBuffer(0, GL_FLOAT, 0, 3);
    myShader->enableAttributeArray(0);
    myShader->setAttributeBuffer(2, GL_FLOAT, vertLen, 3);
    myShader->enableAttributeArray(2);

    myShader->setUniformValue("lightColor", lightColor);
    myShader->setUniformValue("lightPos", lightPos);

    myShader->release();
    myVAO->release();
    myVBO->release();
}

void GLWidget::paintGL() {
    myShader->bind();
    myVAO->bind();

    myShader->setUniformValue("modelMat", modelMatrix);
    myShader->setUniformValue("viewMat", viewMatrix);
    myShader->setUniformValue("projMat", projMatrix);

    myShader->setUniformValue("viewerPos", cameraPos);

//    glDrawElements(GL_TRIANGLE_STRIP, sizeof(indices), GL_UNSIGNED_SHORT, 0);
    for(int i = 0; i < vertexes.size() / 4; ++i)
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);

    myVAO->release();
    myShader->release();
}

void GLWidget::resizeGL(int width, int height) {
    if(!width) width = 1;
    if(!height) height = 1;
    projMatrix.setToIdentity();
    projMatrix.perspective(45.0f, width / float(height), 0.1f, 100.0f);
    glViewport(0, 0, width, height);
}

void GLWidget::myUpdate() {
    if(keys[Qt::Key_W])
        viewMatrix.translate(moveSpeed * myForward);
    if(keys[Qt::Key_S])
        viewMatrix.translate(moveSpeed * myBackward);
    if(keys[Qt::Key_A])
        viewMatrix.translate(moveSpeed * myLeft);
    if(keys[Qt::Key_D])
        viewMatrix.translate(moveSpeed * myRight);
    if(keys[Qt::Key_Q])
        viewMatrix.translate(moveSpeed * myDown);
    if(keys[Qt::Key_E])
        viewMatrix.translate(moveSpeed * myUp);

    if(keys[Qt::Key_K])
        modelMatrix.rotate(-1.0f, QVector3D(1, 0, 0));
    if(keys[Qt::Key_J])
        modelMatrix.rotate(1.0f, QVector3D(1, 0, 0));
    if(keys[Qt::Key_H])
        modelMatrix.rotate(1.0f, QVector3D(0, 1, 0));
    if(keys[Qt::Key_L])
        modelMatrix.rotate(-1.0f, QVector3D(0, 1, 0));

    if(keys[Qt::Key_R] && lightColor.x() < 1.0f)
        lightColor += QVector3D(0.01f, 0, 0);
    if(keys[Qt::Key_T] && lightColor.y() < 1.0f)
        lightColor += QVector3D(0, 0.01f, 0);
    if(keys[Qt::Key_Y] && lightColor.z() < 1.0f)
        lightColor += QVector3D(0, 0, 0.01f);
    if(keys[Qt::Key_U] && lightColor.x() > 0)
        lightColor -= QVector3D(0.01f, 0, 0);
    if(keys[Qt::Key_I] && lightColor.y() > 0)
        lightColor -= QVector3D(0, 0.01f, 0);
    if(keys[Qt::Key_O] && lightColor.z() > 0)
        lightColor -= QVector3D(0, 0, 0.01f);
//    QPointF curPos = cursor().pos();
//    QVector2D deltaPos = QVector2D(curPos - prePos).normalized();
//    deltaPos.normalize();
//    cursor().setPos(this->width() / 2, this->height() / 2);

//    viewCenter += QVector3D(deltaPos.x(), deltaPos.y(), 0) * 0.02f;
//    viewMatrix.setToIdentity();
//    viewMatrix.lookAt(cameraPos, viewCenter, QVector3D(0, 1, 0));

//    cursor().setPos(this->width() / 2, this->height() / 2);
//    prePos = cursor().pos();
//    modelMatrix.rotate(1.0f, QVector3D(0, 1, 0));
    myShader->bind();
    myShader->setUniformValue("lightColor", lightColor);
//    myShader->setUniformValue("lightPos", lightPos);
    myShader->release();

    update();
}

void GLWidget::keyPressEvent(QKeyEvent *event) {
//    switch (event->key()) {
//    case Qt::Key_W:
//        viewMatrix.translate(0, 0, 0.1f);
//        break;
//    case Qt::Key_S:
//        viewMatrix.translate(0, 0, -0.1f);
//        break;
//    case Qt::Key_A:
//        viewMatrix.translate(0.1f, 0, 0);
//        break;
//    case Qt::Key_D:
//        viewMatrix.translate(-0.1f, 0, 0);
//        break;
//    case Qt::Key_H:
//        viewMatrix.rotate(1.0f, QVector3D(0, 1, 0));
//        break;
//    case Qt::Key_L:
//        viewMatrix.rotate(-1.0f, QVector3D(0, 1, 0));
//        break;
//    default:
//        break;
//    }
    int keyCode = event->key();
    if(keyCode < 256)
        keys[keyCode] = true;
}

void GLWidget::keyReleaseEvent(QKeyEvent *event) {
    int keyCode = event->key();
    if(keyCode < 256)
        keys[keyCode] = false;
}
