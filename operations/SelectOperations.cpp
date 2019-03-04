#include "SelectOperations.h"

#include "actions/Action.h"
#include "actions/ActionList.h"

#include "model/Model.h"

#include "command/SelectCommand.h"

SelectOperations::SelectOperations(ActionList *actions, Model *model, QObject *parent)
    : QObject(parent),
      model(model)
{
    allAction = actions->add("Selection", "Select All");
    allAction->setText("&All");
    allAction->setShortcut(QKeySequence("Ctrl+A"));

    connect(allAction, SIGNAL(triggered()), SLOT(selectAll()));

    noneAction = actions->add("Selection", "Select None");
    noneAction->setText("&None");
    noneAction->setShortcut(QKeySequence("Ctrl+D"));

    connect(noneAction, SIGNAL(triggered()), SLOT(selectNone()));

    invertAction = actions->add("Selection", "Invert Selection");
    invertAction->setText("&Invert");
    invertAction->setShortcut(QKeySequence("Ctrl+I"));

    connect(invertAction, SIGNAL(triggered()), SLOT(selectInvert()));

    modelChanged();
    connect(model, SIGNAL(changed()), SLOT(modelChanged()));
    connect(model, SIGNAL(selectionChanged()), SLOT(modelChanged()));
}

void
SelectOperations::modelChanged()
{
    bool any = !model->isEmpty();

    allAction->setEnabled(any);
    noneAction->setEnabled(any);
    invertAction->setEnabled(any);
}

void
SelectOperations::selectAll()
{
    SelectCommand *command = new SelectCommand(model, this);
    command->selectAll();

    model->endCommand(command);
}

void
SelectOperations::selectNone()
{
    SelectCommand *command = new SelectCommand(model, this);
    command->selectNone();

    model->endCommand(command);
}

void
SelectOperations::selectInvert()
{
    SelectCommand *command = new SelectCommand(model, this);
    command->selectInvert();

    model->endCommand(command);
}
