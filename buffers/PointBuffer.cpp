#include "PointBuffer.h"

#include "graphics/PointVertex.h"
#include "graphics/VertexBuffer.h"

#include "model/Model.h"

#include "debug/FunctionTimer.h"

PointBuffer::PointBuffer(Graphics *graphics, Model *model, QObject *parent)
    : Buffer(graphics, 65535 * 2 * sizeof(PointVertex), parent),
      model(model)
{
    connect(model, SIGNAL(changed()), SLOT(invalidate()));
    connect(model, SIGNAL(poseChanged()), SLOT(invalidate()));
    connect(model, SIGNAL(modeChanged()), SLOT(invalidate()));
    connect(model, SIGNAL(selectionChanged()), SLOT(invalidate()));
}

void
PointBuffer::generate(VertexBuffer *buffer)
{
    TimeFunction;

    QVector<int> indices;

    for(int i = 0; i < model->vertexCount(); ++i)
    {
        indices.append(*(model->equivalentVertices(i).begin()));
    }

    buffer->begin(D3DLOCK_DISCARD);

    for(int i = 0; i < indices.count(); ++i)
    {
        if(!model->vertex(indices[i]).hide)
        {
            D3DCOLOR color = model->vertexGroupIncludesSelected(indices[i]) ? D3DCOLOR_XRGB(255, 0, 0) : D3DCOLOR_XRGB(255, 255, 255);

            buffer->add(PointVertex(model->transformedVertexPosition(indices[i]), color));

            if(model->showNormals())
            {
                D3DCOLOR color = model->vertex(indices[i]).manualNormal.valid() ? D3DCOLOR_XRGB(0, 0, 255) : D3DCOLOR_XRGB(0, 255, 0);

                buffer->add(PointVertex(model->transformedVertexPosition(indices[i]) + model->vertexNormal(indices[i]), color));
            }
        }
    }

    buffer->end();
}

