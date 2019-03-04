#ifndef DELETECOMMAND_H
#define DELETECOMMAND_H

#include <QtCore/QVector>
#include <QtCore/QPair>

#include "Command.h"

#include "model/Vertex.h"
#include "model/Face.h"
#include "model/Joint.h"
#include "model/Animation.h"

class DeleteCommand : public Command
{
public:
    DeleteCommand(Model *model, QObject *parent = 0);

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
    activate();

private:
    QVector<QPair<int, Vertex> > verts;
    QVector<QPair<int, Face> > faces;

    QVector<int> jointIndices;
    QVector<QPair<int, Joint> > joints;
    QVector<QPair<int, int> > jointParents;
    QVector<QPair<int, int> > jointPairs;

    QVector<StoredAnimation> animations;

    QVector<QPair<int, QPair<int, VertexInfluence> > > influences;
};

#endif // DELETECOMMAND_H
