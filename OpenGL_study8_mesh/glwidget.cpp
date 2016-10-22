#include "glwidget.h"

GLWidget::GLWidget(QOpenGLWidget *parent)
    : QOpenGLWidget(parent)
{
    QSurfaceFormat mySurface;
    mySurface.setRenderableType(QSurfaceFormat::OpenGL);
    mySurface.setProfile(QSurfaceFormat::CoreProfile);
    mySurface.setVersion(3, 3);

    setFormat(mySurface);

    connect(this, &GLWidget::frameSwapped, this, &GLWidget::updateGL);

    memset(keyState, 0, 256);
}

GLWidget::~GLWidget()
{
    myShaderProg = nullptr;
}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0, 0.1f, 0.1f, 1);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    modelMatrix.setToIdentity();

    myCamera.setPosition(QVector3D(0, 0, 3));
    myCamera.setViewCenter(QVector3D(0, 0, 0));
    myCamera.setUpVector(QVector3D(0, 1, 0));

    vertexes
            //Front
            << Vertex{QVector3D(-0.5f, -0.5f, 0.5f), QVector3D(0, 0, 1), QVector2D(0, 0)}
            << Vertex{QVector3D(0.5f, -0.5f, 0.5f), QVector3D(0, 0, 1), QVector2D(1, 0)}
            << Vertex{QVector3D(0.5f, 0.5f, 0.5f), QVector3D(0, 0, 1), QVector2D(1, 1)}
            << Vertex{QVector3D(-0.5f, 0.5f, 0.5f), QVector3D(0, 0, 1), QVector2D(0, 1)}
           ;

    indices << 0 << 1 << 2 << 2 << 3 << 0;

    texTmp = new QOpenGLTexture(QImage(":/textures/container2.png").mirrored());
    texTmp->setMagnificationFilter(QOpenGLTexture::Linear);
    texTmp->setMinificationFilter(QOpenGLTexture::Linear);
    texTmp->setWrapMode(QOpenGLTexture::ClampToEdge);
    texture.append(Texture{texTmp->textureId(), TexType::Common});

    texTmp = new QOpenGLTexture(QImage(":/textures/container2.png").mirrored());
    texTmp->setMagnificationFilter(QOpenGLTexture::Linear);
    texTmp->setMinificationFilter(QOpenGLTexture::Linear);
    texture.append(Texture{texTmp->textureId(), TexType::Diffuse});

    texTmp = new QOpenGLTexture(QImage(":/textures/container2_specular.png").mirrored());
    texTmp->setMagnificationFilter(QOpenGLTexture::Linear);
    texTmp->setMinificationFilter(QOpenGLTexture::Linear);
    texture.append(Texture{texTmp->textureId(), TexType::Specular});

    mesh = new MyMesh(vertexes, indices, texture);

    mesh->loadVertexShaderFromFile(":shaders/simple.vert");
    mesh->loadFragmentShaderFromFile(":/shaders/simple.frag");

    mesh->prepareToDraw(this->context());

    myShaderProg = mesh->getShaderProg();

    pointLight = {
        QVector3D(0, 1, 2),
        QVector3D(0.8f, 0.8f, 0.8f),
        QVector3D(0.99f, 0.99f, 0.99f),
        QVector3D(1.0f, 1.0f, 1.0f),
        1.0f,
        0.09f,
        0.032f
    };

    myShaderProg->bind();

    myShaderProg->setUniformValue("pointLight.position", pointLight.position);
    myShaderProg->setUniformValue("pointLight.ambient", pointLight.ambient);
    myShaderProg->setUniformValue("pointLight.diffuse", pointLight.diffuse);
    myShaderProg->setUniformValue("pointLight.specular", pointLight.specular);
    myShaderProg->setUniformValue("pointLight.attenuatConstant", pointLight.attenuatConstant);
    myShaderProg->setUniformValue("pointLight.attenuatLinear", pointLight.attenuatLinear);
    myShaderProg->setUniformValue("pointLight.attenuatQuadratic", pointLight.attenuatQuadratic);

    myShaderProg->setUniformValue("material.specShineness", 32.0f);

    myShaderProg->release();
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    myShaderProg->bind();
    myShaderProg->setUniformValue("modelMat", modelMatrix);
    myShaderProg->setUniformValue("viewMat", myCamera.viewMatrix());
    myShaderProg->setUniformValue("projMat", myCamera.projectionMatrix());

    myShaderProg->setUniformValue("viewerPos", myCamera.position());

    mesh->draw();
}

void GLWidget::resizeGL(int width, int height) {
    if(width <= 0) width = 1;
    if(height <= 0) height = 1;

    myCamera.lens()->setPerspectiveProjection(45.0f, float(width) / float(height), 0.1f, 100.0f);
    glViewport(0, 0, width, height);
}

void GLWidget::updateGL() {
//    modelMatrix.rotate(0.5f, QVector3D(0, 1, 0));
    processKey();

    update();
}

void GLWidget::keyPressEvent(QKeyEvent *event) {
    int key = event->key();
    if(key >= 0 && key < 256)
        keyState[key] = true;
}

void GLWidget::keyReleaseEvent(QKeyEvent *event) {
    int key = event->key();
    if(key >= 0 && key < 256)
        keyState[key] = false;
}

void GLWidget::processKey() {
    if(keyState[Qt::Key_J]) {
        modelMatrix.rotate(0.5f, QVector3D(1, 0, 0));
    }
    if(keyState[Qt::Key_K]) {
        modelMatrix.rotate(-0.5f, QVector3D(1, 0, 0));
    }
    if(keyState[Qt::Key_H]) {
        modelMatrix.rotate(0.5f, QVector3D(0, 1, 0));
    }
    if(keyState[Qt::Key_L]) {
        modelMatrix.rotate(-0.5f, QVector3D(0, 1, 0));
    }
}
