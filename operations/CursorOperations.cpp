#include "CursorOperations.h"

#include "actions/Action.h"
#include "actions/ActionList.h"

#include "model/Model.h"

#include "command/MoveCursorCommand.h"
#include "command/TransformCommand.h"
#include "command/CompositeCommand.h"

CursorOperations::CursorOperations(ActionList *actions, Model *model, QObject *parent)
    : QObject(parent),
      model(model)
{
    selectionCentreAction = actions->add("Cursor", "Selection Centre");
    selectionCentreAction->setText("To &Selection Centre");
    selectionCentreAction->setShortcut(QKeySequence("Ctrl+Q"));

    connect(selectionCentreAction, SIGNAL(triggered()), SLOT(selectionCentre()));

    originAction = actions->add("Cursor", "Origin");
    originAction->setText("To &Origin");

    connect(originAction, SIGNAL(triggered()), SLOT(origin()));

    originToCursorAction = actions->add("Cursor", "Origin To Cursor");
    originToCursorAction->setText("Origin to &Cursor");

    connect(originToCursorAction, SIGNAL(triggered()), SLOT(originToCursor()));
}

void
CursorOperations::selectionCentre()
{
    MoveCursorCommand *command = new MoveCursorCommand(model, this);

    command->begin(model->selectionCentre());
    command->finalise();

    model->endCommand(command);
}

void
CursorOperations::origin()
{
    MoveCursorCommand *command = new MoveCursorCommand(model, this);

    command->begin(Vec3(0, 0, 0));
    command->finalise();

    model->endCommand(command);
}

void
CursorOperations::originToCursor()
{
    TransformCommand *transform = new TransformCommand("", model, this);
    transform->setWholeModel();

    transform->transform(translationMatrix(-model->cursor()));
    transform->finalise();

    MoveCursorCommand *command = new MoveCursorCommand(model, this);

    command->begin(Vec3(0, 0, 0));
    command->finalise();

    CompositeCommand *composite = new CompositeCommand("Origin to Cursor", model, this);
    composite->append(transform);
    composite->append(command);

    model->endCommand(composite);
}
