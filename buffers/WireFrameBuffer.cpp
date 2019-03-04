#include "WireFrameBuffer.h"

#include "graphics/PointVertex.h"
#include "graphics/VertexBuffer.h"

#include "model/Model.h"

#include "common/IndexPairKey.h"

#include "debug/FunctionTimer.h"

WireFrameBuffer::WireFrameBuffer(Graphics *graphics, Model *model, QObject *parent)
    : Buffer(graphics, (65535 * 10) * sizeof(PointVertex), parent),
      model(model)
{
    connect(model, SIGNAL(changed()), SLOT(invalidate()));
    connect(model, SIGNAL(poseChanged()), SLOT(invalidate()));
    connect(model, SIGNAL(modeChanged()), SLOT(invalidate()));
    connect(model, SIGNAL(selectionChanged()), SLOT(invalidate()));
}

void
WireFrameBuffer::generate(VertexBuffer *buffer)
{
    TimeFunction;

    QVector<int> indices;

    for(int i = 0; i < model->vertexCount(); ++i)
    {
        indices.append(*(model->equivalentVertices(i).begin()));
    }

    QMap<IndexPairKey, bool> edges;

    for(int i = 0; i < model->faceCount(); ++i)
    {
        if(!model->face(i).hide)
        {
            for(int j = 0; j < 3; ++j)
            {
                int k = (j == 2 ? 0 : j + 1);

                int a = indices[model->face(i).indices[j]];
                int b = indices[model->face(i).indices[k]];

                bool &selected = edges[IndexPairKey(a, b)];

                if(model->face(i).selected)
                {
                    selected = true;
                }
            }
        }
    }

    buffer->begin(D3DLOCK_DISCARD);

    for(QMap<IndexPairKey, bool>::iterator i = edges.begin(); i != edges.end(); ++i)
    {
        D3DCOLOR color = i.value() ? D3DCOLOR_XRGB(255, 0, 0) : D3DCOLOR_XRGB(200, 200, 200);

        buffer->add(PointVertex(model->transformedVertexPosition(i.key().a), color));
        buffer->add(PointVertex(model->transformedVertexPosition(i.key().b), color));

        if(model->showNormals())
        {
            D3DCOLOR color = D3DCOLOR_XRGB(0, 255, 0);

            buffer->add(PointVertex(model->transformedVertexPosition(i.key().a), color));
            buffer->add(PointVertex(model->transformedVertexPosition(i.key().a) + model->vertexNormal(i.key().a), color));

            buffer->add(PointVertex(model->transformedVertexPosition(i.key().b), color));
            buffer->add(PointVertex(model->transformedVertexPosition(i.key().b) + model->vertexNormal(i.key().b), color));
        }
    }

    buffer->end();
}
