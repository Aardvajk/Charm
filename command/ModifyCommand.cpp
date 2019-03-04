#include "ModifyCommand.h"

#include "debug/FunctionTimer.h"

#include "model/Model.h"
#include "model/ModelData.h"

ModifyCommand::ModifyCommand(const QString &name, Model *model, QObject *parent)
    : Command(model, parent),
      commandName(name)
{
}

QString
ModifyCommand::name() const
{
    return commandName;
}

bool
ModifyCommand::isValid() const
{
    return verts.count() || faces.count() || joints.count();
}

bool
ModifyCommand::doesModifyPersistentState() const
{
    return true;
}

void
ModifyCommand::undo()
{
    foreach(const StoredVertex &v, verts)
    {
        modelData()->vertices[v.index] = v.old;
    }

    foreach(const StoredFace &v, faces)
    {
        modelData()->faces[v.index] = v.old;
    }

    foreach(const StoredJoint &v, joints)
    {
        modelData()->joints[v.index] = v.old;
    }

    modelData()->model()->changeSelection();
    modelData()->model()->change();
}

void
ModifyCommand::redo()
{
    activate();
}

void
ModifyCommand::setVertex(const StoredVertex &v)
{
    verts.clear();
    verts.append(v);
}

void
ModifyCommand::setVertices(const QVector<StoredVertex> &v)
{
    verts = v;
}

void
ModifyCommand::setFace(const StoredFace &v)
{
    faces.clear();
    faces.append(v);
}

void
ModifyCommand::setFaces(const QVector<StoredFace> &v)
{
    faces = v;
}

void
ModifyCommand::setJoint(const StoredJoint &v)
{
    joints.clear();
    joints.append(v);
}

void
ModifyCommand::setJoints(const QVector<StoredJoint> &v)
{
    joints = v;
}

void
ModifyCommand::activate()
{
    if(isValid())
    {
        foreach(const StoredVertex &v, verts)
        {
            modelData()->vertices[v.index] = v.value;
        }

        foreach(const StoredFace &v, faces)
        {
            modelData()->faces[v.index] = v.value;
        }

        foreach(const StoredJoint &v, joints)
        {
            modelData()->joints[v.index] = v.value;
        }

        modelData()->model()->changeSelection();
        modelData()->model()->change();
    }
}
