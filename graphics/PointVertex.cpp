#include "PointVertex.h"

QVector<VertexElement>
PointVertex::declaration()
{
    QVector<VertexElement> v;

    v.append(VertexElement(0, 0, D3DDECLTYPE_FLOAT3, D3DDECLUSAGE_POSITION, 0));
    v.append(VertexElement(0, 12, D3DDECLTYPE_FLOAT2, D3DDECLUSAGE_TEXCOORD, 0));
    v.append(VertexElement(0, 20, D3DDECLTYPE_D3DCOLOR, D3DDECLUSAGE_COLOR, 0));
    v.append(VertexElement());

    return v;
}

