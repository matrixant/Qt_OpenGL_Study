#include "openglwidget.h"
#include <QPainter>

OpenGLWidget::OpenGLWidget(QOpenGLWidget *parent)
    : QOpenGLWidget(parent)
{
    QSurfaceFormat mSurface;
    mSurface.setRenderableType(QSurfaceFormat::OpenGL);
    mSurface.setProfile(QSurfaceFormat::CoreProfile);
    mSurface.setVersion(3, 3);

    this->setFormat(mSurface);

    this->show();
}

OpenGLWidget::~OpenGLWidget()
{
    mShader.deleteLater();
    mVBO.destroy();
    mVAO.destroy();
}

void OpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    connect(this, SIGNAL(frameSwapped()), this, SLOT(mUpdate()));
//    QObject::connect(&mTimer, SIGNAL(timeout()), this, SLOT(mUpdate()));

    glEnable(GL_CULL_FACE);

    glClearColor(0, 0, 0.2f, 1);

    modelMatrix.setToIdentity();
    viewMatrix.setToIdentity();
//    modelMatrix.translate(QVector3D(-1.0f, 0, -1.0f));
    modelMatrix.rotate(20.0f, QVector3D(1, 0, 0));
    viewMatrix.lookAt(QVector3D(0, 0, 4), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
    
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
            ;

    mVBO.create();
    mVBO.bind();
    mVBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    mVBO.allocate(vertexes.constData(), vertexes.size() * sizeof(QVector3D));
//    mVBO.allocate(cube_vertexes, sizeof(cube_vertexes));

    mShader.create();
    mShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/cube.vert");
    mShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/cube.frag");
    mShader.link();
    mShader.bind();

    mShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D) * 2);
//    mShader.setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
    mShader.enableAttributeArray(0);

    mShader.setAttributeBuffer(1, GL_FLOAT, sizeof(QVector3D) , 3, sizeof(QVector3D) * 2);
//    mShader.setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());
    mShader.enableAttributeArray(1);


    mVBO.release();
    mVAO.release();
    mShader.release();

//    mTimer.start(100);
}

void OpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mShader.bind();
    mShader.setUniformValue("mvpMat", projMatrix * viewMatrix * modelMatrix);
    mVAO.bind();

    glDrawArrays(GL_TRIANGLES, 0, vertexes.size() / 2);
//    glDrawArrays(GL_TRIANGLES, 0, sizeof(cube_vertexes) / sizeof(cube_vertexes[0]));

    mVAO.release();
    mShader.release();
}

void OpenGLWidget::resizeGL(int width, int height) {
    if(width == 0)  width = 1;
    if(height == 0) height = 1;
    projMatrix.setToIdentity();
    projMatrix.perspective(45.0f, width / float(height), 0.1f, 1000.0f);
    glViewport(0, 0, width, height);
}

void OpenGLWidget::mUpdate() {
//    viewMatrix.translate(QVector3D(0.01f, 0, 0));
    modelMatrix.rotate(1.0f, QVector3D(0, 1.0f, 0));
    QOpenGLWidget::update();
}
