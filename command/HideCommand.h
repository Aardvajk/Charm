#ifndef HIDECOMMAND_H
#define HIDECOMMAND_H

#include <QtCore/QVector>
#include <QtCore/QPair>

#include "Command.h"

#include "model/Vertex.h"
#include "model/Face.h"
#include "model/Joint.h"

class HideCommand : public Command
{
public:
    HideCommand(Model *model, QObject *parent = 0);

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
    setFaces(const QVector<int> &v);

    void
    setJoints(const QVector<int> &v);

    void
    hide();

    void
    unhide();

    void
    unhideAll();

private:
    QString commandName;

    QVector<StoredVertex> verts;
    QVector<StoredFace> faces;
    QVector<StoredJoint> joints;

    QPair<int, int> level;
};

#endif // HIDECOMMAND_H
