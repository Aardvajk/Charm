#include "RawBufferConverter.h"

#include "model/Model.h"

#include "common/DataStream.h"
#include "common/Color.h"

#include <QtCore/QTextStream>
#include <QtCore/QDebug>

RawBufferConverter::RawBufferConverter(bool boneNames, bool faceColors) : boneNames(boneNames), faceColors(faceColors)
{
}

bool
RawBufferConverter::toModel(const QString &path, Model *model)
{
    return false;
}

bool
RawBufferConverter::fromModel(float scale, const Vec3 &offset, const QString &path, Model *model)
{
    DataOutFileStream ds(path.toStdString());
    if(ds.fail())
    {
        err = QString("Unable to create %1").arg(path);
        return false;
    }

    ds << unsigned((model->faceCount() * 3) * 44); // !!! MANUAL VERTEX SIZE
    for(int f = 0; f < model->faceCount(); ++f)
    {
        for(int g = 0; g < 3; ++g)
        {
            int i = model->face(f).indices[g];

            ds << (model->vertex(i).pos * scale) + offset;
            ds << model->vertexNormal(i);

            if(faceColors)
            {
                ds << toD3dColor(model->palette(model->face(f).palette));
            }

            for(int j = 0; j < Vertex::maximumInfluences; ++j)
            {
                if(model->vertex(i).influences[j].joint >= 0)
                {
                    ds << static_cast<unsigned char>(model->vertex(i).influences[j].joint + 1);
                }
                else
                {
                    ds << static_cast<unsigned char>(0);
                }
            }

            for(int j = 0; j < Vertex::maximumInfluences; ++j)
            {
                ds << model->vertex(i).influences[j].weight;
            }
        }
    }

    ds << unsigned(model->jointCount());
    for(int i = 0; i < model->jointCount(); ++i)
    {
        if(boneNames) ds << model->joint(i).details.name.toStdString();
        ds << model->joint(i).pos * scale << static_cast<std::uint8_t>(model->joint(i).details.parent);
    }

    ds << unsigned(model->animationCount());
    for(int i = 0; i < model->animationCount(); ++i)
    {
        ds << model->animation(i).details().name.toStdString() << unsigned(model->animation(i).frames()) << model->animation(i).duration();

        ds << unsigned(model->animation(i).keyFrameCount());
        for(int j = 0; j < model->animation(i).keyFrameCount(); ++j)
        {
            KeyFrame f = model->animation(i).keyFrame(j);

            ds << f.position() - 1.0f;

            ds << unsigned(f.count());
            for(int k = 0; k < f.count(); ++k)
            {
                ds << f.transform(k).rotation << f.transform(k).translation * scale;
            }
        }

        ds << unsigned(model->animation(i).eventCount());
        for(int j = 0; j < model->animation(i).eventCount(); ++j)
        {
            AnimationEvent e = model->animation(i).event(j);

            ds << e.details.position - 1.0f;
            ds << e.details.data.toStdString();
        }
    }

    return true;
}

