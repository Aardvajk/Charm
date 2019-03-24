#include "ModelData.h"

#include <QtCore/QDataStream>
#include <QtCore/QFile>
#include <QtCore/QMap>

ModelData::ModelData(Model *model)
    : hide(0),
      cursor(0, 0, 0),
      parent(model)
{
}

bool
ModelData::isEmpty() const
{
    return vertices.isEmpty() && faces.isEmpty() && joints.isEmpty() && animations.isEmpty() && groups.isEmpty();
}

void
ModelData::clear()
{
    vertices.clear();
    faces.clear();
    joints.clear();
    animations.clear();
    groups.clear();
}

bool
ModelData::loadFromFile(const QString &path)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QDataStream is(&file);

    int version;
    is >> version;

    if(version >= 6)
    {
        is >> lastExportDetails.type;
        is >> lastExportDetails.scale;

        if(version > 9)
        {
            auto &v = lastExportDetails.offset;
            is >> v.x >> v.y >> v.z;
        }
    }

    if(version >= 5)
    {
        is >> lastExportDetails.path;
    }

    int count;

    is >> count;
    for(int i = 0; i < count; ++i)
    {
        Vertex v;
        is >> v;

        if(version >= 2)
        {
            is >> v.group;
        }

        int influences;
        is >> influences;

        for(int j = 0; j < influences; ++j)
        {
            is >> v.influences[j].joint;
            is >> v.influences[j].weight;
        }

        if(version >= 9)
        {
            bool valid;
            is >> valid;

            if(valid)
            {
                Vec3 n;
                is >> n.x >> n.y >> n.z;

                v.manualNormal = n;
            }
        }

        vertices.append(v);
    }

    is >> count;

    for(int i = 0; i < count; ++i)
    {
        Face f;
        is >> f;

        if(version >= 2)
        {
            is >> f.group;
        }

        if(version >= 8)
        {
            is >> f.palette;
        }

        faces.append(f);
    }

    is >> count;
    for(int i = 0; i < count; ++i)
    {
        Joint j;
        is >> j;

        if(version >= 2)
        {
            is >> j.group;
        }

        is >> j.details.name;
        is >> j.details.paired;

        joints.append(j);
    }

    is >> count;
    for(int i = 0; i < count; ++i)
    {
        Animation a;
        a.read(is, version);

        animations.append(a);
    }

    if(version >= 3)
    {
        is >> count;
        for(int i = 0; i < count; ++i)
        {
            Group g(i + 1);
            is >> g.details.name;

            groups.append(g);
        }
    }

    if(version >= 7)
    {
        is >> count;
        for(int i = 0; i < count; ++i)
        {
            int r, g, b;
            is >> r >> g >> b;

            palette.append(QColor(r, g, b));
        }
    }

    return true;
}

bool
ModelData::saveToFile(const QString &path) const
{
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly))
    {
        return false;
    }

    QMap<int, int> groupMap;
    for(int i = 0; i < groups.count(); ++i)
    {
        groupMap[groups[i].id] = i + 1;
    }

    QDataStream os(&file);

    os << 10;

    os << lastExportDetails.type;
    os << lastExportDetails.scale;

    auto v = lastExportDetails.offset;
    os << v.x << v.y << v.z;

    os << lastExportDetails.path;

    os << vertices.count();
    for(int i = 0; i < vertices.count(); ++i)
    {
        os << vertices[i] << groupMap[vertices[i].group];

        os << Vertex::maximumInfluences;
        for(int j = 0; j < Vertex::maximumInfluences; ++j)
        {
            os << vertices[i].influences[j].joint;
            os << vertices[i].influences[j].weight;
        }

        os << vertices[i].manualNormal.valid();
        if(vertices[i].manualNormal.valid())
        {
            Vec3 n = vertices[i].manualNormal.value();

            os << n.x << n.y << n.z;
        }
    }

    os << faces.count();
    for(int i = 0; i < faces.count(); ++i)
    {
        os << faces[i] << groupMap[faces[i].group] << faces[i].palette;
    }

    os << joints.count();
    for(int i = 0; i < joints.count(); ++i)
    {
        os << joints[i] << groupMap[joints[i].group];
        os << joints[i].details.name << joints[i].details.paired;
    }

    os << animations.count();
    for(int i = 0; i < animations.count(); ++i)
    {
        animations[i].write(os, 4);
    }

    os << groups.count();
    for(int i = 0; i < groups.count(); ++i)
    {
        os << groups[i].details.name;
    }

    os << palette.count();
    for(int i = 0; i < palette.count(); ++i)
    {
        os << palette[i].red() << palette[i].green() << palette[i].blue();
    }

    return true;
}
