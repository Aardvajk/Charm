#include "ExtrudeCommand.h"

#include "debug/Assert.h"
#include "debug/FunctionTimer.h"

#include "model/Model.h"
#include "model/ModelData.h"

ExtrudeCommand::ExtrudeCommand(Type type, Model *model, QObject *parent)
    : Command(model, parent),
      type(type),
      valid(false)
{
}

QString
ExtrudeCommand::name() const
{
    return "Extrude";
}

bool
ExtrudeCommand::isValid() const
{
    return valid && edges.count() > 0;
}

bool
ExtrudeCommand::doesModifyPersistentState() const
{
    return true;
}

void
ExtrudeCommand::undo()
{
    TimeFunction;

    if(addedFaces.count())
    {
        Assert(addedFaces.first() >= 0 && addedFaces.first() < modelData()->faces.count() && addedFaces.first() + addedFaces.count() <= modelData()->faces.count());
        modelData()->faces.remove(addedFaces.first(), addedFaces.count());
    }

    for(QMap<int, Face>::const_iterator i = modifiedFaces.begin(); i != modifiedFaces.end(); ++i)
    {
        Assert(i.key() >= 0 && i.key() < modelData()->faces.count());
        modelData()->faces[i.key()] = i.value();
    }

    QList<int> verts = duplicated.values();
    qSort(verts);

    Assert(verts.first() >= 0 && verts.first() < modelData()->vertices.count() && verts.first() + verts.count() <= modelData()->vertices.count());
    modelData()->vertices.remove(verts.first(), verts.count());

    if(type == Edges)
    {
        for(QMap<int, bool>::const_iterator i = verticesSelected.begin(); i != verticesSelected.end(); ++i)
        {
            Assert(i.key() >= 0 && i.key() < modelData()->vertices.count());
            modelData()->vertices[i.key()].selected = i.value();
        }
    }
    else
    {
        for(QMap<int, Vec3>::const_iterator i = modifiedVertices.begin(); i != modifiedVertices.end(); ++i)
        {
            Assert(i.key() >= 0 && i.key() < modelData()->vertices.count());
            modelData()->vertices[i.key()].pos = i.value();
        }
    }

    modelData()->model()->changeSelection();
    modelData()->model()->change();
}

void
ExtrudeCommand::redo()
{
    createGeometry();
    transform(translation);
}

void
ExtrudeCommand::setEdges(const QVector<Edge> &v)
{
    edges.clear();

    if(type == Faces)
    {
        foreach(const Edge &e, v)
        {
            if(!modifiedFaces.contains(e.face))
            {
                Assert(e.face >= 0 && e.face < modelData()->faces.count());
                modifiedFaces[e.face] = modelData()->faces[e.face];
            }

            edges.append(e);
        }
    }
    else
    {
        edges = v;
    }
}

void
ExtrudeCommand::transform(const Matrix &m)
{
    if(!valid)
    {
        createGeometry();
    }

    for(QMap<int, int>::const_iterator i = duplicated.begin(); i != duplicated.end(); ++i)
    {
        Vec3 p = modelData()->vertices[i.key()].pos;

        Assert(i.key() >= 0 && i.key() < modelData()->vertices.count());
        modelData()->vertices[i.value()].pos = transformCoord(p, m);
    }

    for(QMap<int, Vec3>::const_iterator i = modifiedVertices.begin(); i != modifiedVertices.end(); ++i)
    {
        Vec3 p = i.value();

        Assert(i.key() >= 0 && i.key() < modelData()->vertices.count());
        modelData()->vertices[i.key()].pos = transformCoord(p, m);
    }

    translation = m;

    modelData()->model()->change();
}

void
ExtrudeCommand::createGeometry()
{
    duplicated.clear();
    addedFaces.clear();

    foreach(const Edge &e, edges)
    {
        for(int i = 0; i < 2; ++i)
        {
            Assert(e.face >= 0 && e.face < modelData()->faces.count());

            int vert = modelData()->faces[e.face].indices[e.indices[i]];

            if(!duplicated.contains(vert))
            {
                duplicated[vert] = modelData()->vertices.count();

                Assert(vert >= 0 && vert < modelData()->vertices.count());
                modelData()->vertices.append(modelData()->vertices[vert]);
            }
        }
    }

    foreach(const Edge &e, edges)
    {
        Assert(e.face >= 0 && e.face < modelData()->faces.count());

        Face g = modelData()->faces[e.face];

        if(type == Faces)
        {
            modelData()->faces.append(Face(g.indices[e.indices[0]], g.indices[e.indices[1]], duplicated[g.indices[e.indices[0]]]));
            modelData()->faces.append(Face(duplicated[g.indices[e.indices[1]]], duplicated[g.indices[e.indices[0]]], g.indices[e.indices[1]]));
        }
        else
        {
            modelData()->faces.append(Face(g.indices[e.indices[1]], g.indices[e.indices[0]], duplicated[g.indices[e.indices[0]]]));
            modelData()->faces.append(Face(duplicated[g.indices[e.indices[0]]], duplicated[g.indices[e.indices[1]]], g.indices[e.indices[1]]));
        }

        addedFaces.append(modelData()->faces.count() - 2);
        addedFaces.append(modelData()->faces.count() - 1);
    }

    if(type == Faces)
    {
        foreach(int i, modelData()->model()->selectedFaces())
        {
            Assert(i >= 0 && i < modelData()->faces.count());
            Face &f = modelData()->faces[i];

            for(int j = 0; j < 3; ++j)
            {
                if(duplicated.contains(f.indices[j]))
                {
                    f.indices[j] = duplicated[f.indices[j]];
                }
            }
        }

        foreach(int i, modelData()->model()->selectedFaces())
        {
            Assert(i >= 0 && i < modelData()->faces.count());
            Face &f = modelData()->faces[i];

            for(int j = 0; j < 3; ++j)
            {
                if(!duplicated.contains(f.indices[j]))
                {
                    modifiedVertices[f.indices[j]] = modelData()->vertices[f.indices[j]].pos;
                }
            }
        }
    }
    else
    {
        foreach(int i, duplicated.keys())
        {
            Assert(i >= 0 && i < modelData()->vertices.count());

            verticesSelected[i] = modelData()->vertices[i].selected;
            modelData()->vertices[i].selected = false;
        }

        modelData()->model()->changeSelection();
    }

    if(edges.count())
    {
        modelData()->model()->change();
        valid = true;
    }
}
