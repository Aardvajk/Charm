#ifndef BASICVERTEX_H
#define BASICVERTEX_H

#include <QtCore/QVector>

#include <d3dx9.h>

#include "graphics/VertexDeclaration.h"

class BasicVertex
{
public:
    BasicVertex(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &norm, const D3DXVECTOR3 &sharedNorm, const D3DCOLOR &color, const D3DCOLOR &color2) : pos(pos), norm(norm), sharedNorm(sharedNorm), color(color), color2(color2) { }

    D3DXVECTOR3 pos;
    D3DXVECTOR3 norm;
    D3DXVECTOR3 sharedNorm;
    D3DCOLOR color;
    D3DCOLOR color2;

    static
    QVector<VertexElement>
    declaration();
};

#endif // BASICVERTEX_H
