#include "VertexOperations.h"

#include "debug/Assert.h"

#include "common/IndexPairKey.h"

#include "actions/Action.h"
#include "actions/ActionList.h"

#include "model/Model.h"
#include "model/Edge.h"

#include "command/ModifyCommand.h"
#include "command/DeleteCommand.h"
#include "command/CreateCommand.h"
#include "command/CompositeCommand.h"
#include "command/SelectCommand.h"

#include "dialogs/WeightDialog.h"
#include "dialogs/PositionDialog.h"

#include <QtCore/QSet>

VertexOperations::VertexOperations(ActionList *actions, Model *model, QWidget *parent)
    : QObject(parent),
      model(model),
      parentWidget(parent)
{
    axes[0] = axes[1] = axes[2] = true;

    snapAction = actions->add("Vertices", "Snap");
    snapAction->setText("&Snap");
    snapAction->setShortcut(QKeySequence("Ctrl+N"));

    snapAction->setEnabled(false);

    connect(snapAction, SIGNAL(triggered()), SLOT(snap()));

    weldAction = actions->add("Vertices", "Weld");
    weldAction->setText("&Weld");
    weldAction->setShortcut(QKeySequence("Ctrl+W"));

    weldAction->setEnabled(false);

    connect(weldAction, SIGNAL(triggered()), SLOT(weld()));

    splitAction = actions->add("Vertices", "Split");
    splitAction->setText("S&plit");

    splitAction->setEnabled(false);

    connect(splitAction, SIGNAL(triggered()), SLOT(split()));

    splitEdgeAction = actions->add("Vertices", "Split Edge");
    splitEdgeAction->setText("Split &Edge");
    splitEdgeAction->setShortcut(QKeySequence("Ctrl+P"));

    splitEdgeAction->setEnabled(false);

    connect(splitEdgeAction, SIGNAL(triggered()), SLOT(splitEdge()));

    clearWeightsAction = actions->add("Vertices", "Clear Weights");
    clearWeightsAction->setText("Clear &Weights");

    clearWeightsAction->setEnabled(false);

    connect(clearWeightsAction, SIGNAL(triggered()), SLOT(clearWeights()));

    editWeightsAction = actions->add("Vertices", "Edit Weights");
    editWeightsAction->setText("Edit Wei&ghts");

    editWeightsAction->setEnabled(false);

    connect(editWeightsAction, SIGNAL(triggered()), SLOT(editWeights()));

    selectUnassignedAction = actions->add("Vertices", "Select Unassigned");
    selectUnassignedAction->setText("Select &Unassigned");

    selectUnassignedAction->setEnabled(false);

    connect(selectUnassignedAction, SIGNAL(triggered()), SLOT(selectUnassigned()));

    selectUnclosedAction = actions->add("Vertices", "Select Unclosed");
    selectUnclosedAction->setText("Select &Unclosed Edges");

    selectUnclosedAction->setEnabled(false);

    connect(selectUnclosedAction, SIGNAL(triggered()), SLOT(selectUnclosed()));

    positionEditAction = actions->add("Vertices", "Edit Position");
    positionEditAction->setText("Edit Position");

    positionEditAction->setEnabled(false);

    connect(positionEditAction, SIGNAL(triggered()), SLOT(positionEdit()));

    snapToGridAction = actions->add("Vertices", "Snap to Grid");
    snapToGridAction->setText("Snap to Grid");

    snapToGridAction->setEnabled(false);

    connect(snapToGridAction, SIGNAL(triggered()), SLOT(snapToGrid()));

    fixNormalsAction = actions->add("Vertices", "Fix Normals");
    fixNormalsAction->setText("Fix Normals");

    fixNormalsAction->setEnabled(false);

    connect(fixNormalsAction, SIGNAL(triggered()), SLOT(fixNormals()));

    unfixNormalsAction = actions->add("Vertices", "Unfix Normals");
    unfixNormalsAction->setText("Unfix Normals");

    unfixNormalsAction->setEnabled(false);

    connect(unfixNormalsAction, SIGNAL(triggered()), SLOT(unfixNormals()));

    connect(model, SIGNAL(changed()), SLOT(modelChanged()));
    connect(model, SIGNAL(selectionChanged()), SLOT(modelChanged()));
    connect(model, SIGNAL(axisLocked(int,bool)), SLOT(axisLocked(int,bool)));
}

void
VertexOperations::modelChanged()
{
    bool any = model->selectedVertices().count() > 1;

    snapAction->setEnabled(any);
    weldAction->setEnabled(any);

    splitAction->setEnabled(model->selectedVertices().count());

    bool canSplitEdge = false;

    if(model->selectedVertices().count() == 2)
    {
        int a = model->selectedVertices()[0];

        foreach(int f, model->facesContainingVertex(a))
        {
            if(model->face(f).contains(model->selectedVertices()[1]))
            {
                canSplitEdge = true;
                break;
            }
        }
    }

    splitEdgeAction->setEnabled(canSplitEdge);

    clearWeightsAction->setEnabled(model->selectedVertices().count());
    editWeightsAction->setEnabled(model->selectedVertices().count());

    selectUnassignedAction->setEnabled(model->vertexCount() > 0);
    selectUnclosedAction->setEnabled(model->vertexCount() > 0);

    positionEditAction->setEnabled(model->selectedVerticesAllEquivalent());

    snapToGridAction->setEnabled(model->selectedVertices().count());

    fixNormalsAction->setEnabled(model->selectedVertices().count());
    unfixNormalsAction->setEnabled(model->selectedVertices().count());
}

void
VertexOperations::axisLocked(int axis, bool state)
{
    axes[axis] = state;
}

void
VertexOperations::snap()
{
    QVector<StoredVertex> verts;

    Vec3 pos = model->selectionCentre();

    foreach(int i, model->selectedVertices())
    {
        Vertex v = model->vertex(i);
        v.pos = pos;

        verts.append(StoredVertex(i, model->vertex(i), v));
    }

    ModifyCommand *command = new ModifyCommand("Snap", model, this);

    command->setVertices(verts);
    command->activate();

    model->endCommand(command);
}

void
VertexOperations::weld()
{
    QVector<StoredFace> faces;
    QVector<int> verts;

    QMap<int, int> remap;

    foreach(int i, model->selectedVertices())
    {
        if(!verts.contains(i))
        {
            QVector<int> eq = model->equivalentVertices(i);

            if(eq.count() > 1)
            {
                for(int j = 0; j < eq.count(); ++j)
                {
                    int v = eq[j];

                    if(v != i && model->vertex(v).selected)
                    {
                        verts.append(v);
                        remap[v] = i;
                    }
                }
            }
        }
    }

    QSet<int> faceSet;
    foreach(int i, verts)
    {
        foreach(int j, model->facesContainingVertex(i))
        {
            faceSet << j;
        }
    }

    foreach(int i, faceSet)
    {
        Face face = model->face(i);

        for(int j = 0; j < 3; ++j)
        {
            if(remap.contains(face.indices[j]))
            {
                face.indices[j] = remap[face.indices[j]];
            }
        }

        faces.append(StoredFace(i, model->face(i), face));
    }

    ModifyCommand *modify = new ModifyCommand("", model, this);
    modify->setFaces(faces);

    DeleteCommand *del = new DeleteCommand(model, this);
    del->setVertices(verts);

    modify->activate();
    del->activate();

    CompositeCommand *command = new CompositeCommand("Weld", model, this);
    command->append(modify);
    command->append(del);

    model->endCommand(command);
}

void
VertexOperations::split()
{
    QVector<Vertex> verts;
    QMap<int, Face> faceMap;

    foreach(int i, model->selectedVertices())
    {
        QVector<int> faces = model->facesContainingVertex(i);

        for(int j = 1; j < faces.count(); ++j)
        {
            int f = faces[j];

            if(!faceMap.contains(f))
            {
                faceMap[f] = model->face(f);
            }

            faceMap[f].replace(i, model->vertexCount() + verts.count());
            verts.append(model->vertex(i));
        }
    }

    QVector<StoredFace> faces;
    foreach(int i, faceMap.keys())
    {
        faces.append(StoredFace(i, model->face(i), faceMap[i]));
    }

    CreateCommand *create = new CreateCommand("", model, this);
    create->setVertices(verts);

    ModifyCommand *modify = new ModifyCommand("", model, this);
    modify->setFaces(faces);

    create->activate(true);
    modify->activate();

    CompositeCommand *command = new CompositeCommand("Split", model, this);
    command->append(modify);
    command->append(create);

    model->endCommand(command);
}

void
VertexOperations::splitEdge()
{
    if(model->selectedVertices().count() != 2)
    {
        return;
    }

    int a = model->selectedVertices()[0];
    int b = model->selectedVertices()[1];

    Vec3 ap = model->vertex(a).pos;
    Vec3 bp = model->vertex(b).pos;

    Vertex v = model->vertex(a);
    v.pos = Vec3((ap.x + bp.x) / 2, (ap.y + bp.y) / 2, (ap.z + bp.z) / 2);

    QVector<StoredFace> modFaces;
    QVector<Face> addFaces;

    foreach(int f, model->facesContainingVertex(a))
    {
        if(model->face(f).contains(b))
        {
            Face face = model->face(f);
            face.replace(a, model->vertexCount());

            modFaces.append(StoredFace(f, model->face(f), face));

            face = model->face(f);
            face.replace(b, model->vertexCount());

            addFaces.append(face);
        }
    }

    CreateCommand *create = new CreateCommand("", model, this);
    create->setVertices(QVector<Vertex>() << v);
    create->setFaces(addFaces);

    ModifyCommand *modify = new ModifyCommand("", model, this);
    modify->setFaces(modFaces);

    create->activate(false);
    modify->activate();

    CompositeCommand *command = new CompositeCommand("Split Edge", model, this);
    command->append(create);
    command->append(modify);

    model->endCommand(command);
}

void
VertexOperations::editWeights()
{
    Assert(model->selectedVertices().count() > 0);

    WeightDialog dialog(model, WeightInfo::fromSelection(model), parentWidget);

    if(dialog.exec() == QDialog::Accepted)
    {
        WeightInfo info = dialog.weights();

        QVector<StoredVertex> verts;

        foreach(int i, model->selectedVertices())
        {
            Vertex v = model->vertex(i);

            for(int j = 0; j < Vertex::maximumInfluences; ++j)
            {
                v.influences[j] = info.influences[j];
            }

            verts.append(StoredVertex(i, model->vertex(i), v));
        }

        ModifyCommand *command = new ModifyCommand("Edit Weights", model, this);

        command->setVertices(verts);
        command->activate();

        model->endCommand(command);
    }
}

void
VertexOperations::clearWeights()
{
    QVector<StoredVertex> verts;

    foreach(int i, model->selectedVertices())
    {
        Vertex v = model->vertex(i);

        for(int j = 0; j < Vertex::maximumInfluences; ++j)
        {
            v.influences[j].joint = -1;
            v.influences[j].weight = 0;
        }

        verts.append(StoredVertex(i, model->vertex(i), v));
    }

    ModifyCommand *command = new ModifyCommand("Clear Weights", model, this);

    command->setVertices(verts);
    command->activate();

    model->endCommand(command);
}

void
VertexOperations::selectUnassigned()
{
    QVector<int> verts;
    for(int i = 0; i < model->vertexCount(); ++i)
    {
        bool any = false;
        for(int j = 0; j < Vertex::maximumInfluences; ++j)
        {
            if(model->vertex(i).influences[j].joint != -1)
            {
                any = true;
                break;
            }
        }

        if(!any && !model->vertex(i).selected)
        {
            verts.append(i);
        }
    }

    SelectCommand *command = new SelectCommand(model, this);
    command->selectVertices(verts);

    model->endCommand(command);
}

void
VertexOperations::selectUnclosed()
{
    QMap<EdgeKey, Edge> edges;

    for(int f = 0; f < model->faceCount(); ++f)
    {
        Face face = model->face(f);

        for(int v0 = 0; v0 < 3; ++v0)
        {
            int v1 = (v0 < 2 ? v0 + 1 : 0);

            Edge newEdge;

            newEdge.indices[0] = face.indices[v0];
            newEdge.indices[1] = face.indices[v1];

            newEdge.face = f;

            EdgeKey key(face.indices[v0], face.indices[v1]);

            if(!edges.contains(key))
            {
                edges[key] = newEdge;
            }
            else
            {
                edges.remove(key);
            }
        }
    }

    QSet<int> vertexSet;
    foreach(const Edge &edge, edges)
    {
        vertexSet.insert(edge.indices[0]);
        vertexSet.insert(edge.indices[1]);
    }

    QVector<int> verts;
    foreach(int i, vertexSet)
    {
        if(!model->vertex(i).selected)
        {
            verts.append(i);
        }
    }

    SelectCommand *command = new SelectCommand(model, this);
    command->selectVertices(verts);

    model->endCommand(command);
}

void
VertexOperations::positionEdit()
{
    Assert(model->selectedVertices().count());

    PositionDialog dialog(model->vertex(model->selectedVertices().front()).pos, parentWidget);

    if(dialog.exec() == QDialog::Accepted)
    {
        QVector<StoredVertex> verts;

        foreach(int v, model->selectedVertices())
        {
            Vertex vertex = model->vertex(v);
            vertex.pos = dialog.value();

            verts.push_back(StoredVertex(v, model->vertex(v), vertex));
        }

        ModifyCommand *command = new ModifyCommand("Edit Position", model, this);

        command->setVertices(verts);
        command->activate();

        model->endCommand(command);
    }
}

void
VertexOperations::snapToGrid()
{
    Assert(model->selectedVertices().count());

    QVector<StoredVertex> verts;

    foreach(int i, model->selectedVertices())
    {
        Vertex v = model->vertex(i);

        if(axes[0]) v.pos.x = qRound(v.pos.x);
        if(axes[1]) v.pos.y = qRound(v.pos.y);
        if(axes[2]) v.pos.z = qRound(v.pos.z);

        verts.append(StoredVertex(i, model->vertex(i), v));
    }

    ModifyCommand *command = new ModifyCommand("Snap to Grid", model, this);

    command->setVertices(verts);
    command->activate();

    model->endCommand(command);
}

void
VertexOperations::fixNormals()
{
    Assert(model->selectedVertices().count());

    QVector<StoredVertex> verts;

    foreach(int i, model->selectedVertices())
    {
        if(!model->vertex(i).manualNormal.valid())
        {
            Vertex v = model->vertex(i);
            v.manualNormal = model->vertexNormal(i);

            verts.append(StoredVertex(i, model->vertex(i), v));
        }
    }

    ModifyCommand *command = new ModifyCommand("Fix Normals", model, this);

    command->setVertices(verts);
    command->activate();

    model->endCommand(command);
}

void
VertexOperations::unfixNormals()
{
    Assert(model->selectedVertices().count());

    QVector<StoredVertex> verts;

    foreach(int i, model->selectedVertices())
    {
        Vertex v = model->vertex(i);

        v.manualNormal.clear();

        verts.append(StoredVertex(i, model->vertex(i), v));
    }

    ModifyCommand *command = new ModifyCommand("Unfix Normals", model, this);

    command->setVertices(verts);
    command->activate();

    model->endCommand(command);
}

