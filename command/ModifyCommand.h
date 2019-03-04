#ifndef MODIFYCOMMAND_H
#define MODIFYCOMMAND_H

#include <QtCore/QVector>
#include <QtCore/QPair>

#include "Command.h"

#include "model/Vertex.h"
#include "model/Face.h"
#include "model/Joint.h"

class ModifyCommand : public Command
{
public:
    ModifyCommand(const QString &name, Model *model, QObject *parent = 0);

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
    setVertex(const StoredVertex &v);

    void
    setVertices(const QVector<StoredVertex> &v);

    void
    setFace(const StoredFace &v);

    void
    setFaces(const QVector<StoredFace> &v);

    void
    setJoint(const StoredJoint &v);

    void
    setJoints(const QVector<StoredJoint> &v);

    void
    activate();

private:
    QString commandName;

    QVector<StoredVertex> verts;
    QVector<StoredFace> faces;
    QVector<StoredJoint> joints;
};

#endif // MODIFYCOMMAND_H
