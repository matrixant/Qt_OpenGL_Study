#ifndef VERTEX_H
#define VERTEX_H

#include <QtCore>

class Vertex
{
public:
    Vertex();
    Vertex(QVector3D pos, QVector3D normal, QVector3D texCoord);

    int offset();
    int posOffset();
    int normOffset();
    int texOffset();
    float* data();

    QVector3D pos;
    QVector3D normal;
    QVector2D texCoord;
//    QVector<QVector3D> vertData;

    inline int offset() { return sizeof(Vertex); }
};

#endif // VERTEX_H
