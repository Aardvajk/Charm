#include "SupportShape.h"
#include "SupportProvider.h"

#include "command/CreateCommand.h"

#include "common/PositionKey.h"

#include <QtCore/QMap>

namespace
{

QVector<Vec3>
generateNormals(int count)
{
    QVector<Vec3> n;

    const float twoPi = pi() * 2;
    const float ydiff = 1.0f / (count - 1.0f);
    const float xdiff = twoPi / count;

    float y = 0;

    for(int ring = 0; ring < count; ++ring)
    {
        if(ring == 0)
        {
            n.append(Vec3(0, 1, 0));
        }
        else if(ring == count - 1)
        {
            n.append(Vec3(0, -1, 0));
        }
        else
        {
            float xdelta = sinf(y * pi());
            float ydelta = cosf(y * pi());

            float a = 0;
            for(int x = 0; x < count; ++x)
            {
                n.append(Vec3(-sinf(a) * xdelta, ydelta, cosf(a) * xdelta));
                a += xdiff;
            }
        }

        y += ydiff;
    }

    return n;
}

}

void
createSupportShape(const Vec3 &pos, const SupportProvider &support, CreateCommand *command, int existingVertexCount)
{
    int count = 20;

    QVector<Vec3> normals = generateNormals(count);

    QVector<Vertex> verts;

    foreach(const Vec3 &n, normals)
    {
        Vec3 p = support.support(n);
        verts.append(Vertex(pos + p, true));
    }

    QVector<Face> faces;

    for(int ring = 0; ring < count - 1; ++ring)
    {
        if(ring == 0)
        {
            for(int i = 0; i < count; ++i)
            {
                int j = (i < count - 1 ? i + 1 : 0);

                Face face(0, 1 + j, 1 + i);
                face.selected = true;

                faces.append(face);
            }
        }
        else if(ring == count - 2)
        {
            int base = verts.count() - 1;

            for(int i = 0; i < count; ++i)
            {
                int j = (i < count - 1 ? i + 1 : 0);

                Face face(base - count + i, base - count + j, base);
                face.selected = true;

                faces.append(face);
            }
        }
        else
        {
            int vertex = 1 + ((ring - 1) * count);

            for(int i = 0; i < count; ++i)
            {
                int j = (i < count - 1 ? i + 1 : 0);

                Face face(vertex + i, vertex + j, vertex + j + count);
                face.selected = true;

                faces.append(face);

                face = Face(vertex + i, vertex + j + count, vertex + i + count);
                face.selected = true;

                faces.append(face);
            }
        }
    }

    QMap<PositionKey, QVector<int> > mapping;
    for(int i = 0; i < verts.count(); ++i)
    {
        mapping[PositionKey(verts[i].pos)].append(i);
    }

    QVector<int> removeVerts;
    QMap<int, int> remap;

    for(int i = 0; i < verts.count(); ++i)
    {
        if(!removeVerts.contains(i))
        {
            QVector<int> eq = mapping[PositionKey(verts[i].pos)];

            if(eq.count() > 1)
            {
                for(int j = 0; j < eq.count(); ++j)
                {
                    int v = eq[j];

                    if(v != i)
                    {
                        removeVerts.append(v);
                        remap[v] = i;
                    }
                }
            }
        }
    }

    for(int i = 0; i < faces.count(); ++i)
    {
        Face &face = faces[i];

        for(int j = 0; j < 3; ++j)
        {
            if(remap.contains(face.indices[j]))
            {
                face.indices[j] = remap[face.indices[j]];
            }
        }
    }

    int i = 0;
    while(i < faces.count())
    {
        Vec3 a = verts[faces[i].indices[0]].pos;
        Vec3 b = verts[faces[i].indices[1]].pos;
        Vec3 c = verts[faces[i].indices[2]].pos;

        if(triangleArea(a, b, c) < 0.0001f)
        {
            faces.remove(i);
        }
        else
        {
            ++i;
        }
    }

    remap.clear();

    QVector<Vertex> temp;
    for(int i = 0; i < verts.count(); ++i)
    {
        if(!removeVerts.contains(i))
        {
            remap[i] = temp.count();
            temp.append(verts[i]);
        }
    }

    for(int i = 0; i < faces.count(); ++i)
    {
        Face &face = faces[i];

        for(int j = 0; j < 3; ++j)
        {
            if(remap.contains(face.indices[j]))
            {
                face.indices[j] = remap[face.indices[j]];
            }

            face.indices[j] += existingVertexCount;
        }
    }

    qSwap(temp, verts);

    command->setVertices(verts);
    command->setFaces(faces);
}

