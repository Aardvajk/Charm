#include "MoveCursorCommand.h"

#include "debug/Assert.h"

#include "model/Model.h"
#include "model/ModelData.h"

MoveCursorCommand::MoveCursorCommand(Model *model, QObject *parent)
    : Command(model, parent)
{
}

QString
MoveCursorCommand::name() const
{
    return "Move Cursor";
}

bool
MoveCursorCommand::isValid() const
{
    return pos.value != pos.old;
}

bool
MoveCursorCommand::doesModifyPersistentState() const
{
    return false;
}

void
MoveCursorCommand::undo()
{
    modelData()->cursor = pos.old;
    modelData()->model()->changeCursor();
}

void
MoveCursorCommand::redo()
{
    modelData()->cursor = pos.value;
    modelData()->model()->changeCursor();
}

void
MoveCursorCommand::begin(const Vec3 &position)
{
    pos.old = modelData()->cursor;

    modelData()->cursor = position;
    modelData()->model()->changeCursor();
}

void
MoveCursorCommand::update(const Vec3 &position)
{
    modelData()->cursor = position;
    modelData()->model()->changeCursor();
}

void
MoveCursorCommand::finalise()
{
    pos.value = modelData()->cursor;
}

