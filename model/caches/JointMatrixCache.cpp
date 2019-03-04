#include "JointMatrixCache.h"

#include "debug/Assert.h"

#include "model/Model.h"
#include "model/KeyFrame.h"

JointMatrixCache::JointMatrixCache(const Model *model)
    : model(model),
      valid(false)
{
}

void
JointMatrixCache::invalidate()
{
    valid = false;
}

Matrix
JointMatrixCache::matrix(int joint, const KeyFrame &key, bool useKey) const
{
    if(!valid)
    {
        refresh(key, useKey);
        valid = true;
    }

    return data[joint];
}

void
JointMatrixCache::refresh(const KeyFrame &key, bool useKey) const
{
    data.clear();

    for(int i = 0; i < model->jointCount(); ++i)
    {
        data.append(buildMatrix(i, key, useKey));
    }
}

Matrix
JointMatrixCache::buildMatrix(int index, const KeyFrame &key, bool useKey) const
{
    Joint j = model->joint(index);

    if(useKey)
    {
        if(j.details.parent >= 0)
        {
            Assert(key.count() >= model->jointCount());

            int p = j.details.parent;

            Matrix t0 = translationMatrix(-model->joint(p).pos);
            Matrix r = quaternionMatrix(key.transform(index).rotation);
            Matrix t1 = translationMatrix(model->joint(p).pos);

            return (t0 * r * t1) * translationMatrix(key.transform(index).translation) * buildMatrix(p, key, useKey);
        }

        return translationMatrix(key.transform(index).translation);
    }
    else
    {
        if(j.details.parent >= 0)
        {
            int p = j.details.parent;

            Matrix t0 = translationMatrix(-model->joint(p).pos);
            Matrix r = quaternionMatrix(model->joint(index).transform.rotation);
            Matrix t1 = translationMatrix(model->joint(p).pos);

            return (t0 * r * t1) * translationMatrix(j.transform.translation) * buildMatrix(p, key, useKey);
        }

        return translationMatrix(j.transform.translation);
    }
}
