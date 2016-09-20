#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QOpenGLWidget *parent) :
    QOpenGLWidget(parent) {
    QSurfaceFormat mSurface;
    mSurface.setRenderableType(QSurfaceFormat::OpenGL);
    mSurface.setProfile(QSurfaceFormat::CoreProfile);
    mSurface.setVersion(3, 3);

    this->setFormat(mSurface);
    connect(this, &OpenGLWidget::frameSwapped, this, &OpenGLWidget::mUpdate);
}

OpenGLWidget::~OpenGLWidget()
{
    mVAO.destroy();
    mVBO->destroy();
    mShader.deleteLater();
}

void OpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0.2f, 0, 1);

    modelMatrix.setToIdentity();
    modelMatrix.rotate(20.0f, QVector3D(1.0f, 0, 0));
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(QVector3D(0, 0, 3), QVector3D(0, 0, 0), QVector3D(0, 1.0f, 0));

    mTexture = new QOpenGLTexture(QImage(":/texture/container2.png").mirrored());
    mTexture->setMinificationFilter(QOpenGLTexture::Nearest);
    mTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    mTexture->setWrapMode(QOpenGLTexture::Repeat);

    mVAO.create();
    mVAO.bind();
    //vertexes    verts     colors
    vertexes
             //front
             << QVector3D(0.5, 0.5, 0.5) << QVector3D(1, 0, 1) << QVector3D(-0.5, 0.5, 0.5) << QVector3D(1, 0, 1) << QVector3D(-0.5, -0.5, 0.5) << QVector3D(1, 0, 1)
             << QVector3D(-0.5, -0.5, 0.5) << QVector3D(1, 0, 1) << QVector3D(0.5, -0.5, 0.5) << QVector3D(1, 0, 1) << QVector3D(0.5, 0.5, 0.5) << QVector3D(1, 0, 1)
             //left
             << QVector3D(-0.5, 0.5, 0.5) << QVector3D(0, 1, 1) << QVector3D(-0.5, 0.5, -0.5) << QVector3D(0, 1, 1) << QVector3D(-0.5, -0.5, -0.5) << QVector3D(0, 1, 1)
             << QVector3D(-0.5, -0.5, -0.5) << QVector3D(0, 1, 1) << QVector3D(-0.5, -0.5, 0.5) << QVector3D(0, 1, 1) << QVector3D(-0.5, 0.5, 0.5) << QVector3D(0, 1, 1)
             //back
             << QVector3D(-0.5, 0.5, -0.5) << QVector3D(0, 1, 0) << QVector3D(0.5, 0.5, -0.5) << QVector3D(0, 1, 0) << QVector3D(0.5, -0.5, -0.5) << QVector3D(0, 1, 0)
             << QVector3D(0.5, -0.5, -0.5) << QVector3D(0, 1, 0) << QVector3D(-0.5, -0.5, -0.5) << QVector3D(0, 1, 0) << QVector3D(-0.5, 0.5, -0.5) << QVector3D(0, 1, 0)
             //right
             << QVector3D(0.5, 0.5, 0.5) << QVector3D(1, 0, 0) << QVector3D(0.5, -0.5, 0.5) << QVector3D(1, 0, 0) << QVector3D(0.5, -0.5, -0.5) << QVector3D(1, 0, 0)
             << QVector3D(0.5, -0.5, -0.5) << QVector3D(1, 0, 0) << QVector3D(0.5, 0.5, -0.5) << QVector3D(1, 0, 0) << QVector3D(0.5, 0.5, 0.5) << QVector3D(1, 0, 0)
             //top
             << QVector3D(-0.5, 0.5, 0.5) << QVector3D(0, 0, 1) << QVector3D(0.5, 0.5, 0.5) << QVector3D(0, 0, 1) << QVector3D(0.5, 0.5, -0.5) << QVector3D(0, 0, 1)
             << QVector3D(0.5, 0.5, -0.5) << QVector3D(0, 0, 1) << QVector3D(-0.5, 0.5, -0.5) << QVector3D(0, 0, 1) << QVector3D(-0.5, 0.5, 0.5) << QVector3D(0, 0, 1)
             //bottom
             << QVector3D(-0.5, -0.5, 0.5) << QVector3D(1, 1, 0) << QVector3D(-0.5, -0.5, -0.5) << QVector3D(1, 1, 0) << QVector3D(0.5, -0.5, -0.5) << QVector3D(1, 1, 0)
             << QVector3D(0.5, -0.5, -0.5) << QVector3D(1, 1, 0) << QVector3D(0.5, -0.5, 0.5) << QVector3D(1, 1, 0) << QVector3D(-0.5, -0.5, 0.5) << QVector3D(1, 1, 0)
                /*
                         */
            ;
    texCoord
             << QVector2D(0.0f, 0.0f) << QVector2D(1.0f, 0.0f) << QVector2D(1.0f, 1.0f) << QVector2D(1.0f, 1.0f) << QVector2D(0.0f, 1.0f) << QVector2D(0.0f, 0.0f)
             << QVector2D(0.0f, 0.0f) << QVector2D(1.0f, 0.0f) << QVector2D(1.0f, 1.0f) << QVector2D(1.0f, 1.0f) << QVector2D(0.0f, 1.0f) << QVector2D(0.0f, 0.0f)
             << QVector2D(0.0f, 0.0f) << QVector2D(1.0f, 0.0f) << QVector2D(1.0f, 1.0f) << QVector2D(1.0f, 1.0f) << QVector2D(0.0f, 1.0f) << QVector2D(0.0f, 0.0f)
             << QVector2D(0.0f, 0.0f) << QVector2D(1.0f, 0.0f) << QVector2D(1.0f, 1.0f) << QVector2D(1.0f, 1.0f) << QVector2D(0.0f, 1.0f) << QVector2D(0.0f, 0.0f)
             << QVector2D(0.0f, 0.0f) << QVector2D(1.0f, 0.0f) << QVector2D(1.0f, 1.0f) << QVector2D(1.0f, 1.0f) << QVector2D(0.0f, 1.0f) << QVector2D(0.0f, 0.0f)
             << QVector2D(0.0f, 0.0f) << QVector2D(1.0f, 0.0f) << QVector2D(1.0f, 1.0f) << QVector2D(1.0f, 1.0f) << QVector2D(0.0f, 1.0f) << QVector2D(0.0f, 0.0f)
             ;

    mVBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    mVBO->create();
    mVBO->bind();
    mVBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
    mVBO->allocate(vertexes.constData(), vertexes.size() * sizeof(QVector3D) + texCoord.size() * sizeof(QVector2D));
    mVBO->write(vertexes.size() * sizeof(QVector3D), texCoord.constData(), texCoord.size() * sizeof(QVector2D));

    mShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/cube.vert");
    mShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/cube.frag");
    mShader.link();
    mShader.bind();

    mShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D) * 2);
    mShader.enableAttributeArray(0);
    mShader.setAttributeBuffer(1, GL_FLOAT, sizeof(QVector3D), 3, sizeof(QVector3D) * 2);
    mShader.enableAttributeArray(1);
    mShader.setAttributeBuffer(2, GL_FLOAT, vertexes.size() * sizeof(QVector3D), 2);
    mShader.enableAttributeArray(2);

    mShader.release();
    mVBO->release();
    mVAO.release();
}

void OpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mShader.bind();
    mShader.setUniformValue("mvpMat", projectionMatrix * viewMatrix * modelMatrix);
    mVAO.bind();
    mTexture->bind();

    glDrawArrays(GL_TRIANGLES, 0, vertexes.size());

    mVAO.release();
    mShader.release();
}

void OpenGLWidget::resizeGL(int width, int height) {
    if(width == 0) width = 1;
    if(height == 0) height = 1;
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(45.0f, width / float(height), 0.1f, 100.0f);
    glViewport(0, 0, width, height);
}

void OpenGLWidget::mUpdate() {
    modelMatrix.rotate(0.5f, QVector3D(0, 1, 0));
    update();
}
