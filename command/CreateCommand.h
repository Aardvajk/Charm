#ifndef CREATECOMMAND_H
#define CREATECOMMAND_H

#include <QtCore/QVector>

#include "Command.h"

#include "model/Vertex.h"
#include "model/Face.h"
#include "model/Joint.h"

class CreateCommand : public Command
{
public:
    CreateCommand(const QString &name, Model *model, QObject *parent = 0);

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
    setVertex(const Vertex &v);

    void
    setVertices(const QVector<Vertex> &v);

    void
    setFace(const Face &v);

    void
    setFaces(const QVector<Face> &v);

    void
    setJoint(const Joint &v);

    void
    setJoints(const QVector<Joint> &v);

    void
    activate(bool preserve);

private:
    QString commandName;

    QVector<Vertex> verts;
    QVector<Face> faces;
    QVector<Joint> joints;

    QVector<int> deselectVerts;
    QVector<int> deselectFaces;
    QVector<int> deselectJoints;

    bool pres;
};

#endif // CREATECOMMAND_H
