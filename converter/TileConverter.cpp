#include "TileConverter.h"

#include "model/Model.h"

#include "common/DataStream.h"
#include "common/Color.h"

#include <d3dx9.h>

TileConverter::TileConverter()
{
}

bool
TileConverter::toModel(const QString &path, Model *model)
{
    return false;
}

bool
TileConverter::fromModel(float scale, const QString &path, Model *model)
{
    DataOutFileStream ds(path.toStdString());
    if(ds.fail())
    {
        err = QString("Unable to create %1").arg(path);
        return false;
    }

    Vec3 offset(0, 0, 0);
    if(model->jointCount())
    {
        offset = model->joint(0).pos;
    }

    ds << size_t(model->vertexCount());
    for(int v = 0; v < model->vertexCount(); ++v)
    {
        Vec3 pos = (model->vertex(v).pos - offset) * scale;

        ds << pos;

        const QVector<int> &faces = model->facesContainingVertex(v);

        Vec3 sharedNormal(0, 0, 0);
        foreach(int k, faces)
        {
            sharedNormal += model->faceNormal(k);
        }

        if(!faces.empty()) sharedNormal /= faces.count();

        ds << sharedNormal;

        if(faces.empty())
        {
            ds << D3DCOLOR_XRGB(255, 255, 255);
        }
        else
        {
            ds << toD3dColor(model->palette(model->face(faces.front()).palette));
        }

        if(sharedNormal.y > 0.9f)
        {
            float u = (pos.x + 1.0f) / 2.0f;
            float v = (-pos.z + 1.0f) / 2.0f;

            ds << u << v;
        }
        else
        {
            ds << 0.0f << 0.0f;
        }
    }

    ds << size_t(model->faceCount());
    for(int f = 0; f < model->faceCount(); ++f)
    {
        for(int i = 0; i < 3; ++i)
        {
            ds << ushort(model->face(f).indices[i]);
        }
    }

    return true;
}



