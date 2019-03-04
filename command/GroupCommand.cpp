#include "GroupCommand.h"

#include "debug/Assert.h"

#include "model/Model.h"
#include "model/ModelData.h"

GroupCommand::GroupCommand(const QString &name, Model *model, QObject *parent)
    : Command(model, parent),
      commandName(name)
{
}

QString
GroupCommand::name() const
{
    return commandName;
}

bool
GroupCommand::isValid() const
{
    return true;
}

bool
GroupCommand::doesModifyPersistentState() const
{
    return true;
}

void
GroupCommand::undo()
{
    if(stored.old.isNull())
    {
        Assert(stored.index >= 0 && stored.index < modelData()->groups.count());
        modelData()->groups.remove(stored.index);
    }
    else if(stored.value.isNull())
    {
        Assert(stored.index >= 0 && stored.index <= modelData()->groups.count());
        modelData()->groups.insert(stored.index, stored.old);
    }
    else if(!stored.old.isNull() && !stored.value.isNull())
    {
        Assert(stored.index >= 0 && stored.index < modelData()->groups.count());
        modelData()->groups[stored.index] = stored.old;
    }

    foreach(const StoredInt &i, verts)
    {
        Assert(i.index >= 0 && i.index < modelData()->vertices.count());
        modelData()->vertices[i.index].group = i.old;
    }

    foreach(const StoredInt &i, faces)
    {
        Assert(i.index >= 0 && i.index < modelData()->faces.count());
        modelData()->faces[i.index].group = i.old;
    }

    foreach(const StoredInt &i, joints)
    {
        Assert(i.index >= 0 && i.index < modelData()->joints.count());
        modelData()->joints[i.index].group = i.old;
    }

    modelData()->model()->changeGroups();
}

void
GroupCommand::redo()
{
    if(stored.old.isNull())
    {
        modelData()->groups.append(stored.value);
    }
    else if(stored.value.isNull())
    {
        Assert(stored.index >= 0 && stored.index < modelData()->groups.count());
        modelData()->groups.remove(stored.index);
    }
    else if(!stored.old.isNull() && !stored.value.isNull())
    {
        Assert(stored.index >= 0 && stored.index < modelData()->groups.count());
        modelData()->groups[stored.index] = stored.value;
    }

    foreach(const StoredInt &i, verts)
    {
        Assert(i.index >= 0 && i.index < modelData()->vertices.count());
        modelData()->vertices[i.index].group = i.value;
    }

    foreach(const StoredInt &i, faces)
    {
        Assert(i.index >= 0 && i.index < modelData()->faces.count());
        modelData()->faces[i.index].group = i.value;
    }

    foreach(const StoredInt &i, joints)
    {
        Assert(i.index >= 0 && i.index < modelData()->joints.count());
        modelData()->joints[i.index].group = i.value;
    }

    modelData()->model()->changeGroups();
}

void
GroupCommand::add(const Group &group)
{
    stored = StoredGroup(modelData()->groups.count(), Group(), group);

    modelData()->groups.append(group);

    assignToGroup(group.id);

    modelData()->model()->changeGroups();
}

void
GroupCommand::addToGroup(int id)
{
    assignToGroup(id);

    modelData()->model()->changeGroups();
}

void
GroupCommand::modify(int index, const Group &group)
{
    Assert(index >= 0 && index < modelData()->groups.count());
    stored = StoredGroup(index, modelData()->groups[index], group);

    modelData()->groups[index] = group;
    modelData()->model()->changeGroups();
}

void
GroupCommand::remove(int index)
{
    Assert(index >= 0 && index < modelData()->groups.count());
    stored = StoredGroup(index, modelData()->groups[index], Group());

    int id = modelData()->groups[index].id;

    modelData()->groups.remove(index);

    for(int i = 0; i < modelData()->vertices.count(); ++i)
    {
        if(modelData()->vertices[i].group == id)
        {
            verts.append(StoredInt(i, id, 0));
            modelData()->vertices[i].group = 0;
        }
    }

    for(int i = 0; i < modelData()->faces.count(); ++i)
    {
        if(modelData()->faces[i].group == id)
        {
            faces.append(StoredInt(i, id, 0));
            modelData()->faces[i].group = 0;
        }
    }

    for(int i = 0; i < modelData()->joints.count(); ++i)
    {
        if(modelData()->joints[i].group == id)
        {
            joints.append(StoredInt(i, id, 0));
            modelData()->joints[i].group = 0;
        }
    }

    modelData()->model()->changeGroups();
}

void
GroupCommand::assignToGroup(int id)
{
    foreach(int i, modelData()->model()->selectedVertices())
    {
        verts.append(StoredInt(i, modelData()->vertices[i].group, id));
        modelData()->vertices[i].group = id;
    }

    foreach(int i, modelData()->model()->selectedFaces())
    {
        faces.append(StoredInt(i, modelData()->faces[i].group, id));
        modelData()->faces[i].group = id;
    }

    foreach(int i, modelData()->model()->selectedJoints())
    {
        faces.append(StoredInt(i, modelData()->joints[i].group, id));
        modelData()->joints[i].group = id;
    }
}
