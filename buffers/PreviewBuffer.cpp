#include "PreviewBuffer.h"

#include "common/Color.h"

#include "graphics/BasicVertex.h"
#include "graphics/VertexBuffer.h"

#include "model/Model.h"

#include "debug/FunctionTimer.h"

PreviewBuffer::PreviewBuffer(Graphics *graphics, Model *model, QObject *parent)
    : Buffer(graphics, (65535 * 3) * sizeof(BasicVertex), parent),
      model(model)
{
    connect(model, SIGNAL(changed()), SLOT(invalidate()));
    connect(model, SIGNAL(poseChanged()), SLOT(invalidate()));
    connect(model, SIGNAL(modeChanged()), SLOT(invalidate()));
    connect(model, SIGNAL(selectionChanged()), SLOT(invalidate()));
    connect(model, SIGNAL(paletteChanged()), SLOT(invalidate()));
}

void
PreviewBuffer::generate(VertexBuffer *buffer)
{
    TimeFunction;

    buffer->begin(D3DLOCK_DISCARD);

    for(int i = 0; i < model->faceCount(); ++i)
    {
        if(!model->face(i).hide && !model->isFaceDegenerate(i))
        {
            for(int j = 0; j < 3; ++j)
            {
                Vec3 normal = model->vertexNormal(model->face(i).indices[j]);
                int v = model->face(i).indices[j];

                buffer->add(BasicVertex(model->transformedVertexPosition(v), model->faceNormal(i), normal, model->vertexInfluencedColor(model->face(i).indices[j]), toD3dColor(model->palette(model->face(i).palette))));
            }
        }
    }

    buffer->end();
}
