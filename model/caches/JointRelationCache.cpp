#include "JointRelationCache.h"

#include "model/Model.h"

JointRelationCache::JointRelationCache(const Model *model)
    : model(model),
      valid(false)
{
}

void
JointRelationCache::invalidate()
{
    valid = false;
}

const QVector<int>&
JointRelationCache::children(int index) const
{
    if(!valid)
    {
        refresh();
        valid = true;
    }

    return data[index];
}

void
JointRelationCache::refresh() const
{
    data.clear();

    for(int i = 0; i < model->jointCount(); ++i)
    {
        data.append(QVector<int>());
    }

    for(int i = 0; i < model->jointCount(); ++i)
    {
        int p = model->joint(i).details.parent;

        if(p >= 0)
        {
            data[p].append(i);
        }
    }
}

