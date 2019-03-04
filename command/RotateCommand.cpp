#include "RotateCommand.h"

#include "debug/Assert.h"

#include "model/Model.h"
#include "model/ModelData.h"

RotateCommand::RotateCommand(Model *model, QObject *parent)
    : Command(model, parent)
{
}

QString
RotateCommand::name() const
{
    return "Rotate Joints";
}

bool
RotateCommand::isValid() const
{
    return joints.count();
}

bool
RotateCommand::doesModifyPersistentState() const
{
    return false;
}

void
RotateCommand::undo()
{
    for(int i = 0; i < joints.count(); ++i)
    {
        Assert(joints[i].index >= 0 && joints[i].index < modelData()->joints.count());
        modelData()->joints[joints[i].index].transform.rotation = joints[i].old;
    }

    modelData()->model()->change();
}

void
RotateCommand::redo()
{
    for(int i = 0; i < joints.count(); ++i)
    {
        Assert(joints[i].index >= 0 && joints[i].index < modelData()->joints.count());
        modelData()->joints[joints[i].index].transform.rotation = joints[i].value;
    }

    modelData()->model()->change();
}

void
RotateCommand::setJoints(const QVector<int> &v)
{
    foreach(int i, v)
    {
        Assert(i >= 0 && i < modelData()->joints.count());
        joints.append(StoredRotation(i, modelData()->joints[i].transform.rotation, modelData()->joints[i].transform.rotation));
    }
}

void
RotateCommand::transform(const Vec3 &axis, float angle)
{
    foreach(const StoredRotation &r, joints)
    {
        Assert(r.index >= 0 && r.index < modelData()->joints.count());

        Vec3 a = axis;
        int p = modelData()->joints[r.index].details.parent;

        if(p >= 0)
        {
            a = transformNormal(axis, inverseMatrix(modelData()->model()->jointMatrix(p)));
        }

        modelData()->joints[r.index].transform.rotation = r.old * quaternionAxisAngle(a, angle);
    }

    if(joints.count())
    {
        modelData()->model()->change();
        modelData()->model()->setPoseChangedFromKeyFrame(true);
    }
}

void
RotateCommand::transform(const Vec3 &values)
{
    foreach(const StoredRotation &r, joints)
    {
        Assert(r.index >= 0 && r.index < modelData()->joints.count());

        Quaternion q = r.old;

        q = q * quaternionAxisAngle(Vec3(-1, 0, 0), values.x);
        q = q * quaternionAxisAngle(Vec3(0, 1, 0), values.y);
        q = q * quaternionAxisAngle(Vec3(0, 0, 1), -values.z);

        modelData()->joints[r.index].transform.rotation = q;
    }

    if(joints.count())
    {
        modelData()->model()->change();
        modelData()->model()->setPoseChangedFromKeyFrame(true);
    }
}

void
RotateCommand::finalise()
{
    for(int i = 0; i < joints.count(); ++i)
    {
        Assert(joints[i].index >= 0 && joints[i].index < modelData()->joints.count());
        joints[i].value = modelData()->joints[joints[i].index].transform.rotation;
    }
}
