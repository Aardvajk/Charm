#include "SpatialCache.h"

#include "debug/FunctionTimer.h"

#include "model/Model.h"

SpatialCache::SpatialCache(const Model *model)
    : model(model),
      valid(false)
{
}

void
SpatialCache::invalidate()
{
    valid = false;
}

const QVector<int>&
SpatialCache::vertices(int vertex) const
{
    if(!valid)
    {
        refresh();
        valid = true;
    }

    return data[PositionKey(model->transformedVertexPosition(vertex))].first;
}

bool
SpatialCache::selected(int vertex) const
{
    if(!valid)
    {
        refresh();
        valid = true;
    }

    return data[PositionKey(model->transformedVertexPosition(vertex))].second;
}

void
SpatialCache::refresh() const
{
    TimeFunction;

    data.clear();

    for(int i = 0; i < model->vertexCount(); ++i)
    {
        PositionKey key(model->transformedVertexPosition(i));
        data[key].first.append(i);

        if(model->vertex(i).selected)
        {
            data[key].second = true;
        }
    }
}
