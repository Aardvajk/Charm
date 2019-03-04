#ifndef REORDERJOINTCOMMAND_H
#define REORDERJOINTCOMMAND_H

#include "Command.h"

class ReorderJointCommand : public Command
{
public:
    ReorderJointCommand(Model *model, QObject *parent = 0);

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
    activate(int from, int to);

private:
    void
    swap(int from, int to);

    int fromIndex;
    int toIndex;
};

#endif // REORDERJOINTCOMMAND_H
