#include "FaceReferenceCache.h"

#include "debug/Assert.h"
#include "debug/FunctionTimer.h"

#include "model/Model.h"

FaceReferenceCache::FaceReferenceCache(const Model *model)
    : model(model),
      valid(false)
{
}

void
FaceReferenceCache::invalidate()
{
    valid = false;
}

QVector<int>
FaceReferenceCache::faces(int vertex) const
{
    if(!valid)
    {
        update();
        valid = true;
    }

    Assert(vertex >= 0 && vertex < data.count());

    return data[vertex];
}

void
FaceReferenceCache::update() const
{
    TimeFunction;

    data.clear();
    data.resize(model->vertexCount());

    for(int i = 0; i < model->faceCount(); ++i)
    {
        data[model->face(i).indices[0]].append(i);
        data[model->face(i).indices[1]].append(i);
        data[model->face(i).indices[2]].append(i);
    }
}

