#include "KeyFrame.h"

#include "debug/Assert.h"

#include "model/Model.h"
#include "model/ModelData.h"

KeyFrame::KeyFrame(float position)
    : pos(position)
{
}

KeyFrame::KeyFrame(float position, Model *model)
    : pos(position)
{
    for(int i = 0; i < model->jointCount(); ++i)
    {
        transforms.append(model->joint(i).transform);
    }
}

KeyFrame::KeyFrame(Model *model)
    : pos(-1.0f)
{
    for(int i = 0; i < model->jointCount(); ++i)
    {
        transforms.append(model->joint(i).transform);
    }
}

JointTransform
KeyFrame::transform(int index) const
{
    Assert(index >= 0 && index < transforms.count());
    return transforms[index];
}

void
KeyFrame::toModelData(ModelData *data) const
{
    Assert(transforms.count() <= data->joints.count());

    for(int i = 0; i < transforms.count(); ++i)
    {
        data->joints[i].transform = transforms[i];
    }
}

void
KeyFrame::addJoints(int count)
{
    for(int i = 0; i < count; ++i)
    {
        transforms.append(JointTransform());
    }
}

void
KeyFrame::removeJoints(const QVector<int> &indices)
{
    QVector<JointTransform> temp;
    for(int i = 0; i < transforms.count(); ++i)
    {
        if(!indices.contains(i))
        {
            temp.append(transforms[i]);
        }
    }

    transforms = temp;
}

void
KeyFrame::write(QDataStream &stream, int version) const
{
    stream << pos;
    stream << transforms.count();

    for(int i = 0; i < transforms.count(); ++i)
    {
        JointTransform j = transforms[i];
        stream << j.rotation.x << j.rotation.y << j.rotation.z << j.rotation.w;
        stream << j.translation.x << j.translation.y << j.translation.z;
    }
}

void
KeyFrame::read(QDataStream &stream, int version)
{
    stream >> pos;

    int count;
    stream >> count;

    transforms.clear();
    for(int i = 0; i < count; ++i)
    {
        JointTransform j;
        stream >> j.rotation.x >> j.rotation.y >> j.rotation.z >> j.rotation.w;
        stream >> j.translation.x >> j.translation.y >> j.translation.z;

        transforms.append(j);
    }
}

KeyFrame
KeyFrame::interpolate(const KeyFrame &b, float t) const
{
    Assert(transforms.count() == b.transforms.count());

    KeyFrame c;
    for(int i = 0; i < transforms.count(); ++i)
    {
        c.transforms.append(interpolateJointTransforms(transforms[i], b.transforms[i], t));
    }

    return c;
}

void
KeyFrame::swap(int from, int to)
{
    qSwap(transforms[from], transforms[to]);
}
