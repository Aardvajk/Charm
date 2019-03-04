#include "StaticBufferConverter.h"

#include "model/Model.h"

#include "common/DataStream.h"
#include "common/Color.h"

#include <d3dx9.h>

StaticBufferConverter::StaticBufferConverter(bool indexed, bool texCoords, bool dummyTangent) : indexed(indexed), texCoords(texCoords), dummyTangent(dummyTangent)
{
}

bool
StaticBufferConverter::toModel(const QString &path, Model *model)
{
    return false;
}

bool
StaticBufferConverter::fromModel(float scale, const QString &path, Model *model)
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

    if(indexed)
    {
        ds << size_t(model->vertexCount());
        for(int v = 0; v < model->vertexCount(); ++v)
        {
            ds << (model->vertex(v).pos - offset) * scale;
            ds << model->vertexNormal(v);

            const QVector<int> &faces = model->facesContainingVertex(v);

            if(faces.empty())
            {
                ds << D3DCOLOR_XRGB(255, 255, 255);
            }
            else
            {
                ds << toD3dColor(model->palette(model->face(faces.front()).palette));
            }

            if(texCoords)
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
    }
    else
    {
        ds << size_t(model->faceCount() * 3);
        for(int f = 0; f < model->faceCount(); ++f)
        {
            for(int g = 0; g < 3; ++g)
            {
                int i = model->face(f).indices[g];

                ds << (model->vertex(i).pos - offset) * scale;
                ds << model->vertexNormal(i);

                ds << toD3dColor(model->palette(model->face(f).palette));

                if(texCoords)
                {
                    ds << 0.0f << 0.0f;
                }

                if(dummyTangent)
                {
                    ds << 0.0f << 0.0f << 0.0f;
                }
            }
        }
    }

    return true;
}


