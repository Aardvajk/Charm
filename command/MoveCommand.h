#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include "Command.h"

#include "maths/Vec3.h"

#include <QtCore/QVector>

class MoveCommand : public Command
{
public:
    MoveCommand(Model *model, QObject *parent = 0);

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
    setJoints(const QVector<int> &v);

    void
    transform(const Matrix &m);

    void
    finalise();

private:
    QVector<StoredPosition> joints;
};

#endif // MOVECOMMAND_H
