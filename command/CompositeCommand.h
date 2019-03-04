#ifndef COMPOSITECOMMAND_H
#define COMPOSITECOMMAND_H

#include <QtCore/QVector>

#include "Command.h"

class CompositeCommand : public Command
{
public:
    CompositeCommand(const QString &name, Model *model, QObject *parent = 0);

    virtual
    QString
    name() const;

    virtual
    bool
    isValid() const;

    virtual
    bool
    doesModifyPersistentState() const;

    virtual
    void
    undo();

    virtual
    void
    redo();

    void
    append(Command *command);

private:
    QString commandName;
    QVector<Command*> commands;
};

#endif // COMPOSITECOMMAND_H
