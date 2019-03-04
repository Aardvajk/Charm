#include "TransformCommand.h"

#include "debug/Assert.h"
#include "debug/FunctionTimer.h"

#include "model/Model.h"
#include "model/ModelData.h"

TransformCommand::TransformCommand(const QString &name, Model *model, QObject *parent)
    : Command(model, parent),
      commandName(name),
      moved(false)
{
}

QString
TransformCommand::name() const
{
    return commandName;
}

bool
TransformCommand::isValid() const
{
    return (verts.count() || joints.count()) && moved;
}

bool
TransformCommand::doesModifyPersistentState() const
{
    return true;
}

void
TransformCommand::undo()
{
    TimeFunction;

    foreach(const StoredPosition &p, verts)
    {
        Assert(p.index >= 0 && p.index < modelData()->vertices.count());
        modelData()->vertices[p.index].pos = p.old;
    }

    foreach(const StoredPosition &p, joints)
    {
        Assert(p.index >= 0 && p.index < modelData()->joints.count());
        modelData()->joints[p.index].pos = p.old;
    }

    modelData()->model()->change();
}

void
TransformCommand::redo()
{
    TimeFunction;

    foreach(const StoredPosition &p, verts)
    {
        Assert(p.index >= 0 && p.index < modelData()->vertices.count());
        modelData()->vertices[p.index].pos = p.value;
    }

    foreach(const StoredPosition &p, joints)
    {
        Assert(p.index >= 0 && p.index < modelData()->joints.count());
        modelData()->joints[p.index].pos = p.value;
    }

    modelData()->model()->change();
}

void
TransformCommand::setVertices(const QVector<int> &v)
{
    foreach(int index, v)
    {
        Assert(index >= 0 && index < modelData()->vertices.count());
        verts.append(StoredPosition(index, modelData()->vertices[index].pos, modelData()->vertices[index].pos));
    }
}

void
TransformCommand::setJoints(const QVector<int> &v)
{
    foreach(int index, v)
    {
        Assert(index >= 0 && index < modelData()->joints.count());
        joints.append(StoredPosition(index, modelData()->joints[index].pos, modelData()->joints[index].pos));
    }
}

void
TransformCommand::setWholeModel()
{
    for(int i = 0; i < modelData()->vertices.count(); ++i)
    {
        verts.append(StoredPosition(i, modelData()->vertices[i].pos, modelData()->vertices[i].pos));
    }

    for(int i = 0; i < modelData()->joints.count(); ++i)
    {
        joints.append(StoredPosition(i, modelData()->joints[i].pos, modelData()->joints[i].pos));
    }
}

void
TransformCommand::transform(const Matrix &matrix)
{
    TimeFunction;

    foreach(const StoredPosition &p, verts)
    {
        Assert(p.index >= 0 && p.index < modelData()->vertices.count());

        modelData()->vertices[p.index].pos = transformCoord(p.value, matrix);
        moved = true;
    }

    foreach(const StoredPosition &p, joints)
    {
        Assert(p.index >= 0 && p.index < modelData()->joints.count());

        modelData()->joints[p.index].pos = transformCoord(p.value, matrix);
        moved = true;
    }

    modelData()->model()->change();
}

void
TransformCommand::finalise()
{
    for(int i = 0; i < verts.count(); ++i)
    {
        Assert(verts[i].index >= 0 && verts[i].index < modelData()->vertices.count());
        verts[i].value = modelData()->vertices[verts[i].index].pos;
    }

    for(int i = 0; i < joints.count(); ++i)
    {
        Assert(joints[i].index >= 0 && joints[i].index < modelData()->joints.count());
        joints[i].value = modelData()->joints[joints[i].index].pos;
    }
}

