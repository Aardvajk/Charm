#include "UndoList.h"

#include "debug/Assert.h"

#include "command/Command.h"

UndoList::UndoList(QObject *parent)
    : QObject(parent),
      pos(-1),
      save(-1)
{
}

void
UndoList::addCommand(Command *command)
{
    bool last = isModified();

    if(data.count() > pos + 1)
    {
        for(int i = pos + 1; i < data.count(); ++i)
        {
            delete data[i];
        }

        data.remove(pos + 1, data.count() - (pos + 1));
    }

    command->setParent(this);

    data.append(command);
    pos = data.count() - 1;

    emit undoStateChanged();

    if(last != isModified())
    {
        emit modifiedStateChanged(!last);
    }
}

void
UndoList::clear()
{
    qDeleteAll(data);
    data.clear();

    pos = -1;
    save = -1;

    emit undoStateChanged();
    emit modifiedStateChanged(false);
}

bool
UndoList::canUndo() const
{
    return pos >= 0;
}

bool
UndoList::canRedo() const
{
    return pos < data.count() - 1;
}

bool
UndoList::isModified() const
{
    if(save < pos)
    {
        for(int i = save + 1; i <= pos; ++i)
        {
            Assert(i < data.count());

            if(i >= 0 && data[i]->doesModifyPersistentState())
            {
                return true;
            }
        }
    }
    else
    {
        for(int i = save; i > pos; --i)
        {
            Assert(i < data.count());

            if(i >= 0 && data[i]->doesModifyPersistentState())
            {
                return true;
            }
        }
    }

    return false;
}

QString
UndoList::lastCommandName() const
{
    if(canUndo())
    {
        return data[pos]->name();
    }

    return QString();
}

QString
UndoList::nextCommandName() const
{
    if(canRedo())
    {
        return data[pos + 1]->name();
    }

    return QString();
}

void
UndoList::undo()
{
    Assert(canUndo());

    bool last = isModified();

    data[pos--]->undo();

    emit undoStateChanged();
    emit undoTriggered();

    if(last != isModified())
    {
        emit modifiedStateChanged(!last);
    }
}

void
UndoList::redo()
{
    Assert(canRedo());

    bool last = isModified();

    data[++pos]->redo();

    emit undoStateChanged();

    if(last != isModified())
    {
        emit modifiedStateChanged(!last);
    }
}

void
UndoList::setSavePoint()
{
    bool last = isModified();

    save = pos;

    if(last != isModified())
    {
        emit modifiedStateChanged(!last);
    }
}
