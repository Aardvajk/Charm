#include "LevelConverter.h"

#include "model/Model.h"

#include "common/DataStream.h"
#include "common/Color.h"

LevelConverter::LevelConverter()
{
}

bool
LevelConverter::toModel(const QString &path, Model *model)
{
    return false;
}

bool
LevelConverter::fromModel(float scale, const QString &path, Model *model)
{
    DataOutFileStream ds(path.toStdString());
    if(ds.fail())
    {
        err = QString("Unable to create %1").arg(path);
        return false;
    }

    ds << size_t(model->faceCount());
    for(int f = 0; f < model->faceCount(); ++f)
    {
        for(int g = 0; g < 3; ++g)
        {
            int i = model->face(f).indices[g];

            ds << (model->vertex(i).pos) * scale;
            ds << model->vertexNormal(i);
            ds << toD3dColor(model->palette(model->face(f).palette));
            ds << 0.0f << 0.0f;
        }
    }

    return true;
}

