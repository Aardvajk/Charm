#include "HideCommand.h"

#include "model/Model.h"
#include "model/ModelData.h"

HideCommand::HideCommand(Model *model, QObject *parent)
    : Command(model, parent)
{
}

QString
HideCommand::name() const
{
    return commandName;
}

bool
HideCommand::isValid() const
{
    return verts.count() || faces.count() || joints.count();
}

bool
HideCommand::doesModifyPersistentState() const
{
    return false;
}

void
HideCommand::undo()
{
    modelData()->hide = level.first;

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
HideCommand::redo()
{
    modelData()->hide = level.second;

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

void
HideCommand::setVertices(const QVector<int> &v)
{
    foreach(int i, v)
    {
        verts.append(StoredVertex(i, modelData()->vertices[i], modelData()->vertices[i]));
    }
}

void
HideCommand::setFaces(const QVector<int> &v)
{
    foreach(int i, v)
    {
        faces.append(StoredFace(i, modelData()->faces[i], modelData()->faces[i]));
    }
}

void
HideCommand::setJoints(const QVector<int> &v)
{
    foreach(int i, v)
    {
        joints.append(StoredJoint(i, modelData()->joints[i], modelData()->joints[i]));
    }
}

void
HideCommand::hide()
{
    commandName = "Hide";

    level = qMakePair(modelData()->hide, modelData()->hide + 1);

    for(int i = 0; i < verts.count(); ++i)
    {
        modelData()->vertices[verts[i].index].hide = level.second;
        modelData()->vertices[verts[i].index].selected = false;

        verts[i].value = modelData()->vertices[verts[i].index];
    }

    for(int i = 0; i < faces.count(); ++i)
    {
        modelData()->faces[faces[i].index].hide = level.second;
        modelData()->faces[faces[i].index].selected = false;

        faces[i].value = modelData()->faces[faces[i].index];
    }

    for(int i = 0; i < joints.count(); ++i)
    {
        modelData()->joints[joints[i].index].hide = level.second;
        modelData()->joints[joints[i].index].selected = false;

        joints[i].value = modelData()->joints[joints[i].index];
    }

    if(verts.count() || faces.count() || joints.count())
    {
        ++modelData()->hide;

        modelData()->model()->changeSelection();
        modelData()->model()->change();
    }
}

void
HideCommand::unhide()
{
    commandName = "Unhide";

    level = qMakePair(modelData()->hide, modelData()->hide - 1);

    for(int i = 0; i < verts.count(); ++i)
    {
        modelData()->vertices[verts[i].index].hide = 0;
        modelData()->vertices[verts[i].index].selected = true;

        verts[i].value = modelData()->vertices[verts[i].index];
    }

    for(int i = 0; i < faces.count(); ++i)
    {
        modelData()->faces[faces[i].index].hide = 0;
        modelData()->faces[faces[i].index].selected = true;

        faces[i].value = modelData()->faces[faces[i].index];
    }

    for(int i = 0; i < joints.count(); ++i)
    {
        modelData()->joints[joints[i].index].hide = 0;
        modelData()->joints[joints[i].index].selected = true;

        joints[i].value = modelData()->joints[joints[i].index];
    }

    if(verts.count() || faces.count() || joints.count())
    {
        --modelData()->hide;

        modelData()->model()->changeSelection();
        modelData()->model()->change();
    }
}

void
HideCommand::unhideAll()
{
    commandName = "Unhide All";

    level = qMakePair(modelData()->hide, 0);

    for(int i = 0; i < verts.count(); ++i)
    {
        modelData()->vertices[verts[i].index].hide = 0;
        modelData()->vertices[verts[i].index].selected = true;

        verts[i].value = modelData()->vertices[verts[i].index];
    }

    for(int i = 0; i < faces.count(); ++i)
    {
        modelData()->faces[faces[i].index].hide = 0;
        modelData()->faces[faces[i].index].selected = true;

        faces[i].value = modelData()->faces[faces[i].index];
    }

    for(int i = 0; i < joints.count(); ++i)
    {
        modelData()->joints[joints[i].index].hide = 0;
        modelData()->joints[joints[i].index].selected = true;

        joints[i].value = modelData()->joints[joints[i].index];
    }

    if(verts.count() || faces.count() || joints.count())
    {
        modelData()->hide = 0;

        modelData()->model()->changeSelection();
        modelData()->model()->change();
    }
}
