#include "BasicVertex.h"

QVector<VertexElement>
BasicVertex::declaration()
{
    QVector<VertexElement> v;

    v.append(VertexElement(0, 0, D3DDECLTYPE_FLOAT3, D3DDECLUSAGE_POSITION, 0));
    v.append(VertexElement(0, 12, D3DDECLTYPE_FLOAT3, D3DDECLUSAGE_NORMAL, 0));
    v.append(VertexElement(0, 24, D3DDECLTYPE_FLOAT3, D3DDECLUSAGE_NORMAL, 1));
    v.append(VertexElement(0, 36, D3DDECLTYPE_D3DCOLOR, D3DDECLUSAGE_COLOR, 0));
    v.append(VertexElement(0, 40, D3DDECLTYPE_D3DCOLOR, D3DDECLUSAGE_COLOR, 1));
    v.append(VertexElement());

    return v;
}
