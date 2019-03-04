#include "ModifyOperations.h"

#include "common/Axis.h"
#include "common/TransposedName.h"

#include "actions/Action.h"
#include "actions/ActionList.h"

#include "model/Model.h"

#include "command/ModifyCommand.h"
#include "command/CreateCommand.h"
#include "command/CompositeCommand.h"

#include <QtCore/QSet>

ModifyOperations::ModifyOperations(ActionList *actions, Model *model, QObject *parent)
    : QObject(parent),
      model(model)
{
    for(int i = 0; i < static_cast<int>(Axis::None); ++i)
    {
        Axis::Type type = static_cast<Axis::Type>(i);

        flattenAction[i] = actions->add("Modify", QString("Flatten %1").arg(Axis::toString(type)));
        flattenAction[i]->setText(QString("&%1").arg(Axis::toString(type)));
        flattenAction[i]->setData(type);

        flattenAction[i]->setEnabled(false);

        connect(flattenAction[i], SIGNAL(triggered()), SLOT(flatten()));
    }

    for(int i = 0; i < static_cast<int>(Axis::None); ++i)
    {
        Axis::Type type = static_cast<Axis::Type>(i);

        flipAction[i] = actions->add("Modify", QString("Flip %1").arg(Axis::toString(type)));
        flipAction[i]->setText(QString("&%1").arg(Axis::toString(type)));
        flipAction[i]->setData(type);

        flipAction[i]->setEnabled(false);

        connect(flipAction[i], SIGNAL(triggered()), SLOT(flip()));
    }

    for(int i = 0; i < static_cast<int>(Axis::None); ++i)
    {
        Axis::Type type = static_cast<Axis::Type>(i);

        mirrorAction[i] = actions->add("Modify", QString("Mirror %1").arg(Axis::toString(type)));
        mirrorAction[i]->setText(QString("&%1").arg(Axis::toString(type)));
        mirrorAction[i]->setData(type);

        mirrorAction[i]->setEnabled(false);

        connect(mirrorAction[i], SIGNAL(triggered()), SLOT(mirror()));
    }

    for(int i = 0; i < static_cast<int>(Axis::None); ++i)
    {
        Axis::Type type = static_cast<Axis::Type>(i);

        zeroAction[i] = actions->add("Modify", QString("Zero %1").arg(Axis::toString(type)));
        zeroAction[i]->setText(QString("&%1").arg(Axis::toString(type)));
        zeroAction[i]->setData(type);

        zeroAction[i]->setEnabled(false);

        connect(zeroAction[i], SIGNAL(triggered()), SLOT(zero()));
    }

    connect(model, SIGNAL(selectionChanged()), SLOT(modelChanged()));
}

void
ModifyOperations::modelChanged()
{
    int vertices = model->selectedVertices().count();
    int joints = model->selectedJoints().count();

    int allVertices = model->verticesInSelection().count();
    int allJoints = model->jointsInSelection().count();

    for(int i = 0; i < 3; ++i)
    {
        flattenAction[i]->setEnabled(vertices + joints > 1);
        flipAction[i]->setEnabled(allVertices + allJoints > 1);
        mirrorAction[i]->setEnabled(allVertices + allJoints >= 1);
        zeroAction[i]->setEnabled(vertices + joints > 0);
    }
}

void
ModifyOperations::flatten()
{
    int axis = static_cast<Action*>(sender())->data().toInt();

    Vec3 centre = model->referencePoint();

    QVector<StoredVertex> verts;
    foreach(int i, model->selectedVertices())
    {
        Vertex vert = model->vertex(i);
        vert.pos[axis] = centre[axis];

        verts.append(StoredVertex(i, model->vertex(i), vert));
    }

    QVector<StoredJoint> joints;
    foreach(int i, model->selectedJoints())
    {
        Joint joint = model->joint(i);
        joint.pos[axis] = centre[axis];

        joints.append(StoredJoint(i, model->joint(i), joint));
    }

    ModifyCommand *modify = new ModifyCommand("Flatten", model, this);

    modify->setVertices(verts);
    modify->setJoints(joints);

    modify->activate();

    model->endCommand(modify);
}

void
ModifyOperations::flip()
{
    int axis = static_cast<Action*>(sender())->data().toInt();

    float centre = model->referencePoint()[axis];

    QVector<StoredVertex> verts;
    QSet<int> faceSet;

    foreach(int i, model->verticesInSelection())
    {
        Vertex vert = model->vertex(i);
        float v = vert.pos[axis] - centre;

        vert.pos[axis] = (-v) + centre;

        verts.append(StoredVertex(i, model->vertex(i), vert));

        foreach(int f, model->facesContainingVertex(i))
        {
            faceSet.insert(f);
        }
    }

    QVector<StoredFace> faces;
    foreach(int i, faceSet)
    {
        Face face = model->face(i);
        qSwap(face.indices[0], face.indices[1]);

        faces.append(StoredFace(i, model->face(i), face));
    }

    QVector<StoredJoint> joints;
    foreach(int i, model->jointsInSelection())
    {
        Joint joint = model->joint(i);
        float v = joint.pos[axis] - centre;

        joint.pos[axis] = (-v) + centre;

        joints.append(StoredJoint(i, model->joint(i), joint));
    }

    ModifyCommand *modify = new ModifyCommand("Flip", model, this);

    modify->setVertices(verts);
    modify->setFaces(faces);
    modify->setJoints(joints);

    modify->activate();

    model->endCommand(modify);
}

void
ModifyOperations::mirror()
{
    int axis = static_cast<Action*>(sender())->data().toInt();

    float centre = model->referencePoint()[axis];

    QVector<Joint> joints;
    QVector<StoredJoint> modifiedJoints;
    QMap<int, int> jointMap;
    QSet<int> ignoredJoints;

    foreach(int i, model->selectedJoints())
    {
        Joint joint = model->joint(i);

        if(fabs(joint.pos[axis] - centre) < epsilon())
        {
            ignoredJoints.insert(i);
        }
        else
        {
            float v = joint.pos[axis] - centre;
            joint.pos[axis] = (-v) + centre;

            joint.details.paired = i;
            joint.details.name = transposedName(joint.details.name);

            jointMap[i] = model->jointCount() + joints.count();
            joints.append(joint);

            Joint previous = model->joint(i);
            if(previous.details.paired < 0)
            {
                previous.details.paired = jointMap[i];
                previous.selected = false;
                modifiedJoints.append(StoredJoint(i, model->joint(i), previous));
            }
        }
    }

    for(int i = 0; i < joints.count(); ++i)
    {
        int p = joints[i].details.parent;

        if(!ignoredJoints.contains(p))
        {
            if(p >= 0 && jointMap.contains(p))
            {
                joints[i].details.parent = jointMap[p];
            }
            else
            {
                joints[i].details.parent = -1;
            }
        }
    }

    QMap<int, int> vertMap;
    QSet<int> ignoredVerts;
    int writtenVerts = 0;

    QVector<Vertex> verts;
    foreach(int i, model->verticesInSelection())
    {
        Vertex vert = model->vertex(i);

        if(fabs(vert.pos[axis] - centre) < epsilon())
        {
            ignoredVerts.insert(i);
        }
        else
        {
            float v = vert.pos[axis] - centre;
            vert.pos[axis] = (-v) + centre;

            for(int j = 0; j < Vertex::maximumInfluences; ++j)
            {
                int n = vert.influences[j].joint;

                if(jointMap.contains(n))
                {
                    vert.influences[j].joint = jointMap[n];
                }
            }

            verts.append(vert);

            vertMap[i] = model->vertexCount() + writtenVerts;
            ++writtenVerts;
        }
    }

    QVector<Face> faces;
    foreach(int i, model->selectedFaces())
    {
        Face face = model->face(i);

        for(int j = 0; j < 3; ++j)
        {
            if(!ignoredVerts.contains(face.indices[j]))
            {
                face.indices[j] = vertMap[face.indices[j]];
            }
        }

        qSwap(face.indices[0], face.indices[1]);

        faces.append(face);
    }

    CreateCommand *create = new CreateCommand("", model, this);

    create->setVertices(verts);
    create->setFaces(faces);
    create->setJoints(joints);

    create->activate(false);

    ModifyCommand *modify = 0;

    if(modifiedJoints.count())
    {
        modify = new ModifyCommand("", model, this);

        modify->setJoints(modifiedJoints);
        modify->activate();
    }

    CompositeCommand *composite = new CompositeCommand("Mirror", model, this);

    composite->append(create);
    if(modify)
    {
        composite->append(modify);
    }

    model->endCommand(composite);
}

void
ModifyOperations::zero()
{
    int axis = static_cast<Action*>(sender())->data().toInt();

    QVector<StoredVertex> verts;
    foreach(int i, model->selectedVertices())
    {
        Vertex vert = model->vertex(i);
        vert.pos[axis] = 0;

        verts.append(StoredVertex(i, model->vertex(i), vert));
    }

    QVector<StoredJoint> joints;
    foreach(int i, model->selectedJoints())
    {
        Joint joint = model->joint(i);
        joint.pos[axis] = 0;

        joints.append(StoredJoint(i, model->joint(i), joint));
    }

    ModifyCommand *modify = new ModifyCommand("Flatten", model, this);

    modify->setVertices(verts);
    modify->setJoints(joints);

    modify->activate();

    model->endCommand(modify);
}
