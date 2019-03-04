#include "ClipboardOperations.h"

#include "actions/Action.h"
#include "actions/ActionList.h"

#include "model/Model.h"

#include <QtWidgets/QApplication>
#include <QtGui/QClipboard>

#include <QtCore/QDataStream>
#include <QtCore/QMimeData>

#include "command/DeleteCommand.h"
#include "command/CreateCommand.h"

namespace
{

const QString mimeType = "aardvajk/charm/modeldata";

}

ClipboardOperations::ClipboardOperations(ActionList *actions, Model *model, QObject *parent)
    : QObject(parent),
      model(model)
{
    cutAction = actions->add("Model", "Cut");
    cutAction->setText("Cu&t");
    cutAction->setShortcut(QKeySequence("Ctrl+X"));

    cutAction->setEnabled(false);

    connect(cutAction, SIGNAL(triggered()), SLOT(cut()));

    copyAction = actions->add("Model", "Copy");
    copyAction->setText("&Copy");
    copyAction->setShortcut(QKeySequence("Ctrl+C"));

    copyAction->setEnabled(false);

    connect(copyAction, SIGNAL(triggered()), SLOT(copy()));

    pasteAction = actions->add("Model", "Paste");
    pasteAction->setText("&Paste");
    pasteAction->setShortcut(QKeySequence("Ctrl+V"));

    pasteAction->setEnabled(hasFormat());

    connect(pasteAction, SIGNAL(triggered()), SLOT(paste()));

    connect(model, SIGNAL(selectionChanged()), SLOT(modelChanged()));

    connect(QApplication::clipboard(), SIGNAL(dataChanged()), SLOT(clipboardChanged()));
}

void
ClipboardOperations::modelChanged()
{
    cutAction->setEnabled(model->anySelection());
    copyAction->setEnabled(model->anySelection());
}

void
ClipboardOperations::clipboardChanged()
{
    pasteAction->setEnabled(hasFormat());
}

void
ClipboardOperations::cut()
{
    copy();

    DeleteCommand *command = new DeleteCommand(model, this);

    command->setVertices(model->selectedVertices());
    command->setFaces(model->selectedFaces());
    command->setJoints(model->selectedJoints());

    command->activate();

    model->endCommand(command);
}

void
ClipboardOperations::copy()
{
    QByteArray bytes = createMimeData();

    if(!bytes.isEmpty())
    {
        QMimeData *data = new QMimeData();
        data->setData(mimeType, bytes);

        QApplication::clipboard()->setMimeData(data);
    }
}

void
ClipboardOperations::paste()
{
    const QMimeData *data = QApplication::clipboard()->mimeData();
    QByteArray bytes = data->data(mimeType);

    Vec3 cursor = model->referencePoint();

    if(!bytes.isEmpty())
    {
        QVector<Vertex> verts;
        QVector<Face> faces;
        QVector<Joint> joints;

        QDataStream in(&bytes, QIODevice::ReadOnly);

        int count;

        in >> count;
        for(int i = 0; i < count; ++i)
        {
            Vertex v;
            in >> v;

            for(int j = 0; j < Vertex::maximumInfluences; ++j)
            {
                in >> v.influences[j].joint;

                if(v.influences[j].joint >= 0)
                {
                    v.influences[j].joint += model->jointCount();
                }

                in >> v.influences[j].weight;
            }

            v.pos += cursor;
            v.selected = true;

            verts.append(v);
        }

        in >> count;
        for(int i = 0; i < count; ++i)
        {
            Face f;
            in >> f;

            for(int j = 0; j < 3; ++j)
            {
                f.indices[j] += model->vertexCount();
            }

            f.selected = true;
            faces.append(f);
        }

        in >> count;
        for(int i = 0; i < count; ++i)
        {
            Joint j;
            in >> j;

            in >> j.details.name;
            in >> j.details.paired;

            j.pos += cursor;

            if(j.details.parent >= 0)
            {
                j.details.parent += model->jointCount();
            }

            if(j.details.paired >= 0)
            {
                j.details.paired += model->jointCount();
            }

            j.selected = true;
            joints.append(j);
        }

        for(int i = 0; i < verts.count(); ++i)
        {
            for(int j = 0; j < Vertex::maximumInfluences; ++j)
            {
                if(verts[i].influences[j].joint >= model->jointCount() + joints.count())
                {
                    verts[i].influences[j].joint = 0;
                }
            }
        }

        CreateCommand *command = new CreateCommand("Paste", model, this);

        command->setVertices(verts);
        command->setFaces(faces);
        command->setJoints(joints);

        command->activate(false);
        model->endCommand(command);
    }
}

QByteArray
ClipboardOperations::createMimeData()
{
    QMap<int, int> vertMap;

    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);

    Vec3 centre = model->selectionCentre();

    QMap<int, int> jointMap;
    QVector<Joint> joints;

    foreach(int i, model->selectedJoints())
    {
        Joint j = model->joint(i);
        j.pos -= centre;

        joints.append(j);
        jointMap[i] = joints.count() - 1;
    }

    int writtenVerts = 0;

    out << model->verticesInSelection().count();
    foreach(int i, model->verticesInSelection())
    {
        Vertex v = model->vertex(i);
        v.pos -= centre;

        out << v;

        for(int j = 0; j < Vertex::maximumInfluences; ++j)
        {
            int joint = v.influences[j].joint;

            if(jointMap.contains(joint))
            {
                joint = jointMap[joint];
            }

            out << joint;
            out << v.influences[j].weight;
        }

        vertMap[i] = writtenVerts++;
    }

    out << model->selectedFaces().count();
    foreach(int i, model->selectedFaces())
    {
        Face f = model->face(i);

        for(int j = 0; j < 3; ++j)
        {
            if(vertMap.contains(f.indices[j]))
            {
                f.indices[j] = vertMap[f.indices[j]];
            }
        }

        out << f;
    }

    for(int i = 0; i < joints.count(); ++i)
    {
        QMap<int, int>::const_iterator m0 = jointMap.find(joints[i].details.parent);

        if(m0 != jointMap.end())
        {
            joints[i].details.parent = m0.value();
        }
        else
        {
            joints[i].details.parent = -1;
        }

        QMap<int, int>::const_iterator m1 = jointMap.find(joints[i].details.paired);

        if(m1 != jointMap.end())
        {
            joints[i].details.paired = m1.value();
        }
        else
        {
            joints[i].details.paired = -1;
        }
    }

    out << joints.count();
    foreach(const Joint &joint, joints)
    {
        out << joint;
        out << joint.details.name;
        out << joint.details.paired;
    }

    return data;
}

bool
ClipboardOperations::hasFormat() const
{
    const QMimeData *data = QApplication::clipboard()->mimeData();
    return data && data->hasFormat(mimeType);
}
