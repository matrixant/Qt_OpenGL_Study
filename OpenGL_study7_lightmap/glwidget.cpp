#include "glwidget.h"
#include <QDebug>
#include <QtMath>

GLWidget::GLWidget(QOpenGLWidget *parent) :
    QOpenGLWidget(parent)
{
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

    Material material = {
        QVector3D(0.7f, 0.7f, 0.7f),
        QVector3D(1.0f, 1.0f, 1.0f),
        32.0f
    };

    pointLight = {
        QVector3D(0, 1, 2),
        QVector3D(0.8f, 0.8f, 0.8f),
        QVector3D(0.99f, 0.99f, 0.99f),
        QVector3D(1.0f, 1.0f, 1.0f),
        1.0f,
        0.09f,
        0.032f
    };

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
    texCoords
            << QVector2D(0, 0) << QVector2D(1, 0) << QVector2D(1, 1) << QVector2D(0, 1)
            << QVector2D(0, 0) << QVector2D(1, 0) << QVector2D(1, 1) << QVector2D(0, 1)
            << QVector2D(0, 0) << QVector2D(1, 0) << QVector2D(1, 1) << QVector2D(0, 1)
            << QVector2D(0, 0) << QVector2D(1, 0) << QVector2D(1, 1) << QVector2D(0, 1)
            << QVector2D(0, 0) << QVector2D(1, 0) << QVector2D(1, 1) << QVector2D(0, 1)
            << QVector2D(0, 0) << QVector2D(1, 0) << QVector2D(1, 1) << QVector2D(0, 1)
            ;
    int texCoordLen = texCoords.size() * sizeof(QVector2D);

    myVBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    myVBO->create();
    myVBO->bind();
    myVBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
    myVBO->allocate(vertexes.constData(), vertLen + normalLen + texCoordLen);
    myVBO->write(vertLen, normals.constData(), normalLen);
    myVBO->write(vertLen + normalLen, texCoords.constData(), texCoordLen);

    myShader = new QOpenGLShaderProgram;
    myShader->create();
    myShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/cube.vert");
    myShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/cube.frag");
    myShader->link();
    myShader->bind();

    myShader->setAttributeBuffer(0, GL_FLOAT, 0, 3);
    myShader->enableAttributeArray(0);
    myShader->setAttributeBuffer(1, GL_FLOAT, vertLen, 3);
    myShader->enableAttributeArray(1);
    myShader->setAttributeBuffer(2, GL_FLOAT, vertLen + normalLen, 2);
    myShader->enableAttributeArray(2);

    myShader->setUniformValue("pointLight.position", pointLight.position);
    myShader->setUniformValue("pointLight.ambient", pointLight.ambient);
    myShader->setUniformValue("pointLight.diffuse", pointLight.diffuse);
    myShader->setUniformValue("pointLight.specular", pointLight.specular);
    myShader->setUniformValue("pointLight.attenuatConstant", pointLight.attenuatConstant);
    myShader->setUniformValue("pointLight.attenuatLinear", pointLight.attenuatLinear);
    myShader->setUniformValue("pointLight.attenuatQuadratic", pointLight.attenuatQuadratic);

    myShader->setUniformValue("material.specShininess", material.specShininess);

    texture_diff = new QOpenGLTexture(QImage(":/textures/container2.png").mirrored());
    texture_diff->setMagnificationFilter(QOpenGLTexture::Nearest);
    texture_diff->setMinificationFilter(QOpenGLTexture::Nearest);
    texture_diff->setWrapMode(QOpenGLTexture::ClampToEdge);

    texture_spec = new QOpenGLTexture(QImage(":/textures/container2_specular.png").mirrored());
    texture_spec->setMagnificationFilter(QOpenGLTexture::Nearest);
    texture_spec->setMinificationFilter(QOpenGLTexture::Nearest);

//    texture_diff->boundTextureId(0, QOpenGLTexture::BindingTarget2D);
//    texture_spec->boundTextureId(1, QOpenGLTexture::BindingTarget2D);

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(myShader->uniformLocation("material.diffuse"), 0);
    glBindTexture(GL_TEXTURE_2D, texture_diff->textureId());

    glActiveTexture(GL_TEXTURE1);
    glUniform1i(myShader->uniformLocation("material.specular"), 1);
    glBindTexture(GL_TEXTURE_2D, texture_spec->textureId());

//    texture_diff->bind(0);
//    texture_spec->bind(1);

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

//    texture_diff->bind(0);
//    texture_spec->bind(1);
    glBindTexture(GL_TEXTURE_2D, texture_diff->textureId());
    glBindTexture(GL_TEXTURE_2D, texture_spec->textureId());
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

    update();
}

void GLWidget::keyPressEvent(QKeyEvent *event) {
    int keyCode = event->key();
    if(keyCode < 256)
        keys[keyCode] = true;
}

void GLWidget::keyReleaseEvent(QKeyEvent *event) {
    int keyCode = event->key();
    if(keyCode < 256)
        keys[keyCode] = false;
}
