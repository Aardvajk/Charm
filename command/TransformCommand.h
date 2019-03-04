#ifndef TRANSFORMCOMMAND_H
#define TRANSFORMCOMMAND_H

#include "Command.h"

#include "maths/Matrix.h"

#include <QtCore/QVector>

class TransformCommand : public Command
{
    Q_OBJECT

public:
    TransformCommand(const QString &name, Model *model, QObject *parent = 0);

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
    setVertices(const QVector<int> &v);

    void
    setJoints(const QVector<int> &v);

    void
    setWholeModel();

    void
    transform(const Matrix &matrix);

    void
    finalise();

private:
    QString commandName;

    QVector<StoredPosition> verts;
    QVector<StoredPosition> joints;

    bool moved;
};

#endif // TRANSFORMCOMMAND_H
