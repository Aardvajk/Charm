#ifndef ROTATECOMMAND_H
#define ROTATECOMMAND_H

#include "Command.h"

#include "model/Joint.h"

#include <QtCore/QVector>

class RotateCommand : public Command
{
public:
    RotateCommand(Model *model, QObject *parent = 0);

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
    transform(const Vec3 &axis, float angle);

    void
    transform(const Vec3 &values);

    void
    finalise();

private:
    QVector<StoredRotation> joints;
};

#endif // ROTATECOMMAND_H
