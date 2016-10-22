#include "vertex.h"

Vertex::Vertex()
{
}

int Vertex::offset() {
    return sizeof(Vertex);
}

int Vertex::posOffset() {
    return offsetof(Vertex, pos);
}

int Vertex::normOffset() {
    return offsetof(Vertex, normal);
}
