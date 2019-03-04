#include "FaceOperations.h"

#include "actions/Action.h"
#include "actions/ActionList.h"

#include "model/Model.h"

#include "command/CreateCommand.h"
#include "command/ModifyCommand.h"
#include "command/SelectCommand.h"
#include "command/CompositeCommand.h"

FaceOperations::FaceOperations(ActionList *actions, Model *model, QObject *parent)
    : QObject(parent),
      model(model)
{
    createAction = actions->add("Faces", "Create");
    createAction->setText("Create &Face");
    createAction->setShortcut(QKeySequence("F"));

    createAction->setEnabled(false);

    connect(createAction, SIGNAL(triggered()), SLOT(create()));

    reverseAction = actions->add("Faces", "Reverse");
    reverseAction->setText("&Reverse Vertex Order");
    reverseAction->setShortcut(QKeySequence("Ctrl+F"));

    reverseAction->setEnabled(false);

    connect(reverseAction, SIGNAL(triggered()), SLOT(reverse()));

    selectVertsAction = actions->add("Faces", "Select Vertices");
    selectVertsAction->setText("&Select Vertices");

    selectVertsAction->setEnabled(false);

    connect(selectVertsAction, SIGNAL(triggered()), SLOT(selectVerts()));

    groupSplitAction = actions->add("Faces", "Group Split");
    groupSplitAction->setText("&Group Split");

    groupSplitAction->setEnabled(false);

    connect(groupSplitAction, SIGNAL(triggered()), SLOT(groupSplit()));

    selectDegeneratesAction = actions->add("Faces", "Select Degenerates");
    selectDegeneratesAction->setText("Select &Degenerates");

    connect(selectDegeneratesAction, SIGNAL(triggered()), SLOT(selectDegenerates()));

    clearPaintingAction = actions->add("Faces", "Clear Painting");
    clearPaintingAction->setText("Clear Painting");

    connect(clearPaintingAction, SIGNAL(triggered()), SLOT(clearPainting()));

    connect(model, SIGNAL(selectionChanged()), SLOT(modelChanged()));
    connect(model, SIGNAL(modeChanged()), SLOT(modelChanged()));
}

void
FaceOperations::modelChanged()
{
    createAction->setEnabled(model->selectedVertices().count() == 3 && model->mode() == Mode::Model);
    reverseAction->setEnabled(model->selectedFaces().count());
    selectVertsAction->setEnabled(model->selectedFaces().count());
    groupSplitAction->setEnabled(model->selectedFaces().count());
    clearPaintingAction->setEnabled(model->selectedFaces().count());
}

void
FaceOperations::create()
{
    Face face;
    face.selected = true;

    int n = 0;
    foreach(int i, model->selectedVertices())
    {
        face.indices[n++] = i;
    }

    CreateCommand *command = new CreateCommand("Create Face", model, this);
    command->setFace(face);

    command->activate(true);

    model->endCommand(command);
}

void
FaceOperations::reverse()
{
    QVector<StoredFace> faces;

    foreach(int i, model->selectedFaces())
    {
        Face f = model->face(i);
        qSwap(f.indices[0], f.indices[1]);

        faces.append(StoredFace(i, model->face(i), f));
    }

    ModifyCommand *command = new ModifyCommand("Reverse Vertex Order", model, this);
    command->setFaces(faces);

    command->activate();

    model->endCommand(command);
}

void
FaceOperations::selectVerts()
{
    QVector<int> verts;

    foreach(int i, model->selectedFaces())
    {
        for(int j = 0; j < 3; ++j)
        {
            int v = model->face(i).indices[j];

            if(!model->vertex(v).selected && !verts.contains(v))
            {
                verts.append(v);
            }
        }
    }

    SelectCommand *command = new SelectCommand(model, this);
    command->selectVertices(verts);

    model->endCommand(command);
}

void
FaceOperations::groupSplit()
{
    QVector<Vertex> verts;
    QVector<StoredFace> modifiedFaces;
    QVector<StoredVertex> modifiedVerts;

    QMap<int, int> remap;

    QSet<int> selectedSet;
    for(int i = 0; i < model->vertexCount(); ++i)
    {
        bool selected = false;
        bool unselected = false;

        foreach(int j, model->facesContainingVertex(i))
        {
            if(model->face(j).selected)
            {
                selectedSet.insert(j);
                selected = true;
            }
            else
            {
                unselected = true;
            }
        }

        if(selected && unselected)
        {
            remap[i] = model->vertexCount() + verts.count();

            verts.append(model->vertex(i));
            verts.back().selected = true;

            Vertex vertex = model->vertex(i);
            vertex.selected = false;

            modifiedVerts.append(StoredVertex(i, model->vertex(i), vertex));
        }
    }

    foreach(int i, selectedSet)
    {
        Face face = model->face(i);

        for(int j = 0; j < 3; ++j)
        {
            if(remap.contains(face.indices[j]))
            {
                face.indices[j] = remap[face.indices[j]];
            }
        }

        modifiedFaces.append(StoredFace(i, model->face(i), face));
    }

    CreateCommand *create = new CreateCommand("", model, this);
    create->setVertices(verts);
    create->activate(true);

    ModifyCommand *modify = new ModifyCommand("", model, this);
    modify->setVertices(modifiedVerts);
    modify->setFaces(modifiedFaces);
    modify->activate();

    CompositeCommand *command = new CompositeCommand("Group Split", model, this);
    command->append(create);
    command->append(modify);

    model->endCommand(command);
}

void
FaceOperations::selectDegenerates()
{
    QVector<int> faces;

    for(int i = 0; i < model->faceCount(); ++i)
    {
        Vec3 a = model->vertex(model->face(i).indices[0]).pos;
        Vec3 b = model->vertex(model->face(i).indices[1]).pos;
        Vec3 c = model->vertex(model->face(i).indices[2]).pos;

        if(triangleArea(a, b, c) < epsilon())
        {
            faces.append(i);
        }
    }

    SelectCommand *command = new SelectCommand(model, this);
    command->selectFaces(faces);

    model->endCommand(command);
}

void
FaceOperations::clearPainting()
{
    QVector<StoredFace> faces;

    for(int i = 0; i < model->selectedFaces().count(); ++i)
    {
        Face face = model->face(model->selectedFaces()[i]);
        if(face.palette >= 0)
        {
            Face old = face;
            face.palette = -1;

            faces.push_back(StoredFace(model->selectedFaces()[i], old, face));
        }
    }

    if(!faces.isEmpty())
    {
        ModifyCommand *command = new ModifyCommand("Clear Painting", model, this);

        command->setFaces(faces);
        command->activate();

        model->endCommand(command);
    }
}
