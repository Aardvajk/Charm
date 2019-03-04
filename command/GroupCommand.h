#ifndef GROUPCOMMAND_H
#define GROUPCOMMAND_H

#include "Command.h"

#include "model/Group.h"

#include <QtCore/QVector>

class GroupCommand : public Command
{
public:
    GroupCommand(const QString &name, Model *model, QObject *parent = 0);

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
    add(const Group &group);

    void
    addToGroup(int id);

    void
    modify(int index, const Group &group);

    void
    remove(int index);

private:
    void
    assignToGroup(int id);

    QString commandName;
    StoredGroup stored;

    QVector<StoredInt> verts;
    QVector<StoredInt> faces;
    QVector<StoredInt> joints;
};

#endif // GROUPCOMMAND_H
