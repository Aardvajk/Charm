#include "JointBuffer.h"

#include "graphics/VertexBuffer.h"
#include "graphics/PointVertex.h"

#include "maths/Maths.h"

JointBuffer::JointBuffer(Graphics *graphics, float radius, D3DCOLOR color, QObject *parent)
    : Buffer(graphics, sizeof(PointVertex) * 120, parent),
      radius(radius),
      color(color)
{
}

void
JointBuffer::generate(VertexBuffer *buffer)
{
    QVector<Vec3> pts;

    buffer->begin(D3DLOCK_DISCARD);

    pts.clear();
    for(float a = 0; a < pi() * 2.0f; a += pi() / 10.0f)
    {
        pts.append(Vec3(sinf(a) * radius, cosf(a) * radius, 0));
    }

    for(int i = 0; i < pts.count(); ++i)
    {
        int j = i < pts.count() - 1 ? i + 1 : 0;

        buffer->add(PointVertex(pts[i], color));
        buffer->add(PointVertex(pts[j], color));
    }

    pts.clear();
    for(float a = 0; a < pi() * 2.0f; a += pi() / 10.0f)
    {
        pts.append(Vec3(sinf(a) * radius, 0, cosf(a) * radius));
    }

    for(int i = 0; i < pts.count(); ++i)
    {
        int j = i < pts.count() - 1 ? i + 1 : 0;

        buffer->add(PointVertex(pts[i], color));
        buffer->add(PointVertex(pts[j], color));
    }

    pts.clear();
    for(float a = 0; a < pi() * 2.0f; a += pi() / 10.0f)
    {
        pts.append(Vec3(0, sinf(a) * radius, cosf(a) * radius));
    }

    for(int i = 0; i < pts.count(); ++i)
    {
        int j = i < pts.count() - 1 ? i + 1 : 0;

        buffer->add(PointVertex(pts[i], color));
        buffer->add(PointVertex(pts[j], color));
    }

    buffer->end();
}
