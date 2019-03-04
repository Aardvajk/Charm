#include "DeleteOperations.h"

#include "actions/Action.h"
#include "actions/ActionList.h"

#include "model/Model.h"

#include "command/DeleteCommand.h"

DeleteOperations::DeleteOperations(ActionList *actions, Model *model, QObject *parent)
    : QObject(parent),
      model(model)
{
    delAction = actions->add("Model", "Delete");
    delAction->setText("&Delete");
    delAction->setShortcut(QKeySequence("Del"));

    delAction->setEnabled(false);

    connect(delAction, SIGNAL(triggered()), SLOT(del()));

    connect(model, SIGNAL(selectionChanged()), SLOT(modelChanged()));
}

void
DeleteOperations::modelChanged()
{
    delAction->setEnabled(model->anySelection());
}

void
DeleteOperations::del()
{
    DeleteCommand *command = new DeleteCommand(model, this);

    command->setVertices(model->selectedVertices());
    command->setFaces(model->selectedFaces());
    command->setJoints(model->selectedJoints());

    command->activate();

    model->endCommand(command);
}
