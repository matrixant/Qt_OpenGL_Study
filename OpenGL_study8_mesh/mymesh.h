#ifndef MYMESH_H
#define MYMESH_H

#include <QObject>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

struct Vertex {
    QVector3D position;
    QVector3D normal;
    QVector2D texCoord;
};

enum TexType {
    Common,
    Diffuse,
    Specular,
    Normal
};

struct Texture {
    uint id;
    TexType type;
};

class MyMesh : public QObject
{
    Q_OBJECT
public:
    QVector<Vertex> vertexes;
    QVector<uint> indices;
    QVector<Texture> texture;

    MyMesh(const QVector<Vertex>& vertexes, const QVector<quint32>& indices, const QVector<Texture>& texture);
    ~MyMesh();

    void loadVertexShaderFromFile(const QString& file);
    void loadFragmentShaderFromFile(const QString& file);
    QOpenGLShaderProgram* getShaderProg();
    void prepareToDraw(QOpenGLContext *context);
    void draw();


private:
    QOpenGLFunctions *myGLFunc;
    QOpenGLShaderProgram *shaderProg;
    QOpenGLBuffer myGLBuffer;
    QOpenGLVertexArrayObject myVertexArray;

    const int normalOffset = 12;
    const int texCoordOffset = 24;
    const int stride = 32;

    void setupMesh();
};

#endif // MYMESH_H
