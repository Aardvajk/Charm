#include "HideOperations.h"

#include "actions/Action.h"
#include "actions/ActionList.h"

#include "model/Model.h"

#include "command/HideCommand.h"

HideOperations::HideOperations(ActionList *actions, Model *model, QObject *parent)
    : QObject(parent),
      model(model)
{
    hideAction = actions->add("Model", "Hide");
    hideAction->setText("&Hide");
    hideAction->setShortcut(QKeySequence("Ctrl+H"));

    hideAction->setEnabled(false);

    connect(hideAction, SIGNAL(triggered()), SLOT(hide()));

    unhideAction = actions->add("Model", "Unhide");
    unhideAction->setText("&Unhide");
    unhideAction->setShortcut(QKeySequence("Ctrl+Shift+H"));

    unhideAction->setEnabled(false);

    connect(unhideAction, SIGNAL(triggered()), SLOT(unhide()));

    unhideAllAction = actions->add("Model", "Unhide All");
    unhideAllAction->setText("Unhide &All");

    unhideAllAction->setEnabled(false);

    connect(unhideAllAction, SIGNAL(triggered()), SLOT(unhideAll()));

    connect(model, SIGNAL(selectionChanged()), SLOT(modelChanged()));
}

void
HideOperations::modelChanged()
{
    hideAction->setEnabled(model->anySelection());
    unhideAction->setEnabled(model->hide() > 0);
    unhideAllAction->setEnabled(model->hide() > 0);
}

void
HideOperations::hide()
{
    HideCommand *command = new HideCommand(model, this);

    command->setVertices(model->selectedVertices());
    command->setFaces(model->selectedFaces());
    command->setJoints(model->selectedJoints());

    command->hide();
    model->endCommand(command);
}

void
HideOperations::unhide()
{
    if(model->hide() == 0)
    {
        return;
    }

    QVector<int> verts;
    QVector<int> faces;
    QVector<int> joints;

    for(int i = 0; i < model->vertexCount(); ++i)
    {
        if(model->vertex(i).hide == model->hide())
        {
            verts.append(i);
        }
    }

    for(int i = 0; i < model->faceCount(); ++i)
    {
        if(model->face(i).hide == model->hide())
        {
            faces.append(i);
        }
    }

    for(int i = 0; i < model->jointCount(); ++i)
    {
        if(model->joint(i).hide == model->hide())
        {
            joints.append(i);
        }
    }

    HideCommand *command = new HideCommand(model, this);

    command->setVertices(verts);
    command->setFaces(faces);
    command->setJoints(joints);

    command->unhide();
    model->endCommand(command);
}

void
HideOperations::unhideAll()
{
    if(model->hide() == 0)
    {
        return;
    }

    QVector<int> verts;
    QVector<int> faces;
    QVector<int> joints;

    for(int i = 0; i < model->vertexCount(); ++i)
    {
        if(model->vertex(i).hide)
        {
            verts.append(i);
        }
    }

    for(int i = 0; i < model->faceCount(); ++i)
    {
        if(model->face(i).hide)
        {
            faces.append(i);
        }
    }

    for(int i = 0; i < model->jointCount(); ++i)
    {
        if(model->joint(i).hide)
        {
            joints.append(i);
        }
    }

    HideCommand *command = new HideCommand(model, this);

    command->setVertices(verts);
    command->setFaces(faces);
    command->setJoints(joints);

    command->unhideAll();
    model->endCommand(command);
}
