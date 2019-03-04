#ifndef POINTVERTEX_H
#define POINTVERTEX_H

#include <QtCore/QVector>

#include <d3dx9.h>

#include "graphics/VertexDeclaration.h"

class PointVertex
{
public:
    PointVertex(){ }
    PointVertex(const D3DXVECTOR3 &pos, D3DCOLOR color, float u = 0, float v = 0) : pos(pos), u(u), v(v), color(color) { }

    D3DXVECTOR3 pos;
    float u,v;
    D3DCOLOR color;

    static
    QVector<VertexElement>
    declaration();
};

#endif // POINTVERTEX_H
