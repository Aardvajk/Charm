#include "CompositeCommand.h"

CompositeCommand::CompositeCommand(const QString &name, Model *model, QObject *parent)
    : Command(model, parent),
      commandName(name)
{
}

QString
CompositeCommand::name() const
{
    return commandName;
}

bool
CompositeCommand::isValid() const
{
    foreach(const Command *command, commands)
    {
        if(command->isValid())
        {
            return true;
        }
    }

    return false;
}

bool
CompositeCommand::doesModifyPersistentState() const
{
    foreach(const Command *command, commands)
    {
        if(command->doesModifyPersistentState())
        {
            return true;
        }
    }

    return false;
}

void
CompositeCommand::undo()
{
    for(int i = commands.count() - 1; i >= 0; --i)
    {
        commands[i]->undo();
    }
}

void
CompositeCommand::redo()
{
    for(int i = 0; i < commands.count(); ++i)
    {
        commands[i]->redo();
    }
}

void
CompositeCommand::append(Command *command)
{
    command->setParent(this);
    commands.append(command);
}
