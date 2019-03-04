#include "CreateCommand.h"

#include "model/Model.h"
#include "model/ModelData.h"

CreateCommand::CreateCommand(const QString &name, Model *model, QObject *parent)
    : Command(model, parent),
      commandName(name)
{
}

QString
CreateCommand::name() const
{
    return commandName;
}

bool
CreateCommand::isValid() const
{
    return verts.count() || faces.count() || joints.count();
}

bool
CreateCommand::doesModifyPersistentState() const
{
    return true;
}

void
CreateCommand::undo()
{
    int first = modelData()->joints.count() - joints.count();

    modelData()->vertices.remove(modelData()->vertices.count() - verts.count(), verts.count());
    modelData()->faces.remove(modelData()->faces.count() - faces.count(), faces.count());
    modelData()->joints.remove(modelData()->joints.count() - joints.count(), joints.count());

    if(joints.count())
    {
        QVector<int> indices;

        for(int i = 0; i < joints.count(); ++i)
        {
            indices.append(first + i);
        }

        for(int i = 0; i < modelData()->animations.count(); ++i)
        {
            modelData()->animations[i].removeJoints(indices);
        }
    }

    foreach(int i, deselectVerts)
    {
        modelData()->vertices[i].selected = true;
    }

    foreach(int i, deselectFaces)
    {
        modelData()->faces[i].selected = true;
    }

    foreach(int i, deselectJoints)
    {
        modelData()->joints[i].selected = true;
    }

    modelData()->model()->changeSelection();
    modelData()->model()->change();
}

void
CreateCommand::redo()
{
    activate(pres);
}

void
CreateCommand::setVertex(const Vertex &v)
{
    verts.clear();
    verts.append(v);
}

void
CreateCommand::setVertices(const QVector<Vertex> &v)
{
    verts = v;
}

void
CreateCommand::setFace(const Face &v)
{
    faces.clear();
    faces.append(v);
}

void
CreateCommand::setFaces(const QVector<Face> &v)
{
    faces = v;
}

void
CreateCommand::setJoint(const Joint &v)
{
    joints.clear();
    joints.append(v);
}

void
CreateCommand::setJoints(const QVector<Joint> &v)
{
    joints = v;
}

void
CreateCommand::activate(bool preserve)
{
    if(isValid())
    {
        pres = preserve;

        if(!preserve)
        {
            foreach(int i, modelData()->model()->selectedVertices())
            {
                modelData()->vertices[i].selected = false;
                deselectVerts.append(i);
            }

            foreach(int i, modelData()->model()->selectedFaces())
            {
                modelData()->faces[i].selected = false;
                deselectFaces.append(i);
            }

            foreach(int i, modelData()->model()->selectedJoints())
            {
                modelData()->joints[i].selected = false;
                deselectJoints.append(i);
            }
        }

        modelData()->vertices << verts;
        modelData()->faces << faces;
        modelData()->joints << joints;

        if(joints.count())
        {
            for(int i = 0; i < modelData()->animations.count(); ++i)
            {
                modelData()->animations[i].addJoints(joints.count());
            }
        }

        modelData()->model()->changeSelection();
        modelData()->model()->change();
    }
}
