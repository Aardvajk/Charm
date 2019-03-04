#include "GroupCache.h"

#include "model/Model.h"

GroupCache::GroupCache(const Model *model)
    : model(model),
      valid(false)
{
}

void
GroupCache::invalidate()
{
    valid = false;
}

int
GroupCache::index(int id) const
{
    if(!valid)
    {
        update();
        valid = true;
    }

    return idMap[id];
}

bool
GroupCache::containsId(int id) const
{
    if(!valid)
    {
        update();
        valid = true;
    }

    return idMap.contains(id);
}

int
GroupCache::count(int id) const
{
    if(!valid)
    {
        update();
        valid = true;
    }

    return countMap[id];
}

void
GroupCache::update() const
{
    idMap.clear();
    for(int i = 0; i < model->groupCount(); ++i)
    {
        idMap[model->group(i).id] = i;
    }

    countMap.clear();

    for(int i = 0; i < model->vertexCount(); ++i)
    {
        if(model->vertex(i).group)
        {
            ++countMap[model->vertex(i).group];
        }
    }

    for(int i = 0; i < model->faceCount(); ++i)
    {
        if(model->face(i).group)
        {
            ++countMap[model->face(i).group];
        }
    }

    for(int i = 0; i < model->jointCount(); ++i)
    {
        if(model->joint(i).group)
        {
            ++countMap[model->joint(i).group];
        }
    }
}
