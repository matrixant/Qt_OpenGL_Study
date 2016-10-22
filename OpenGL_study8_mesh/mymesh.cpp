#include "mymesh.h"
#include <QDebug>

MyMesh::MyMesh(const QVector<Vertex>& vertexes, const QVector<quint32>& indices, const QVector<Texture>& texture) {
    this->vertexes = vertexes;
    this->indices = indices;
    this->texture = texture;

    shaderProg = new QOpenGLShaderProgram(this);
}

MyMesh::~MyMesh() {
    delete myGLFunc;
    delete shaderProg;
    myGLBuffer.destroy();
    myVertexArray.destroy();
}

void MyMesh::prepareToDraw(QOpenGLContext *context) {
    myGLFunc = new QOpenGLFunctions(context);
    shaderProg->link();

    setupMesh();

}

void MyMesh::draw() {
    shaderProg->bind();
    myVertexArray.bind();
    {
        QString texUniformName;
        for(int i = 0; i < texture.size(); ++i) {
            myGLFunc->glActiveTexture(GL_TEXTURE0 + i);
            switch(texture[i].type) {
            case TexType::Common:
                texUniformName = "mtexture";
                break;
            case TexType::Diffuse:
                texUniformName = "material.diffuse";
                break;
            case TexType::Specular:
                texUniformName = "material.specular";
                break;
            default:
                break;
            }
            myGLFunc->glUniform1f(shaderProg->uniformLocation(texUniformName), i);
            myGLFunc->glBindTexture(GL_TEXTURE_2D, texture[i].id);
        }

        myGLFunc->glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.constData());
    }
    myVertexArray.release();
}

void MyMesh::setupMesh() {
    myVertexArray.create();
    myVertexArray.bind();
    {
        myGLBuffer.create();
        myGLBuffer.bind();
        myGLBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
        qint32 vertsCount = vertexes.size() * sizeof(Vertex);
        myGLBuffer.allocate(vertexes.constData(), vertsCount);

        shaderProg->bind();
        {
            qint8 posId = shaderProg->attributeLocation("position");
            shaderProg->setAttributeBuffer(posId, GL_FLOAT, 0, 3, stride);
            shaderProg->enableAttributeArray(posId);
            qint8 normId = shaderProg->attributeLocation("normal");
            shaderProg->setAttributeBuffer(normId, GL_FLOAT, normalOffset, 3, stride);
            shaderProg->enableAttributeArray(normId);
            qint8 texCoordId = shaderProg->attributeLocation("texCoord");
            shaderProg->setAttributeBuffer(texCoordId, GL_FLOAT, texCoordOffset, 2, stride);
            shaderProg->enableAttributeArray(texCoordId);
        }
        shaderProg->release();
    }
    myVertexArray.release();
}

void MyMesh::loadVertexShaderFromFile(const QString &file) {
    shaderProg->addShaderFromSourceFile(QOpenGLShader::Vertex, file);
}

void MyMesh::loadFragmentShaderFromFile(const QString &file) {
    shaderProg->addShaderFromSourceFile(QOpenGLShader::Fragment, file);
}

QOpenGLShaderProgram* MyMesh::getShaderProg() {
    return this->shaderProg;
}
