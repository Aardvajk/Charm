#include "MoveCommand.h"

#include "debug/Assert.h"

#include "model/Model.h"
#include "model/ModelData.h"

MoveCommand::MoveCommand(Model *model, QObject *parent)
    : Command(model, parent)
{
}

QString
MoveCommand::name() const
{
    return "Move Joints";
}

bool
MoveCommand::isValid() const
{
    return joints.count();
}

bool
MoveCommand::doesModifyPersistentState() const
{
    return false;
}

void
MoveCommand::undo()
{
    for(int i = 0; i < joints.count(); ++i)
    {
        Assert(joints[i].index >= 0 && joints[i].index < modelData()->joints.count());
        modelData()->joints[joints[i].index].transform.translation = joints[i].old;
    }

    modelData()->model()->change();
}

void
MoveCommand::redo()
{
    for(int i = 0; i < joints.count(); ++i)
    {
        Assert(joints[i].index >= 0 && joints[i].index < modelData()->joints.count());
        modelData()->joints[joints[i].index].transform.translation = joints[i].value;
    }

    modelData()->model()->change();
}

void
MoveCommand::setJoints(const QVector<int> &v)
{
    foreach(int i, v)
    {
        Assert(i >= 0 && i < modelData()->joints.count());
        joints.append(StoredPosition(i, modelData()->joints[i].transform.translation, modelData()->joints[i].transform.translation));
    }
}

void
MoveCommand::transform(const Matrix &m)
{
    foreach(const StoredPosition &r, joints)
    {
        Assert(r.index >= 0 && r.index < modelData()->joints.count());

        int p = modelData()->joints[r.index].details.parent;

        Matrix n = m;

        if(p >= 0)
        {
            n *= inverseMatrix(modelData()->model()->jointMatrix(p));
        }

        modelData()->joints[r.index].transform.translation = transformCoord(r.old, n);
    }

    if(joints.count())
    {
        modelData()->model()->change();
        modelData()->model()->setPoseChangedFromKeyFrame(true);
    }
}

void
MoveCommand::finalise()
{
    for(int i = 0; i < joints.count(); ++i)
    {
        Assert(joints[i].index >= 0 && joints[i].index < modelData()->joints.count());
        joints[i].value = modelData()->joints[joints[i].index].transform.translation;
    }
}
