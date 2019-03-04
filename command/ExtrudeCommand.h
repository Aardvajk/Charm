#ifndef EXTRUDECOMMAND_H
#define EXTRUDECOMMAND_H

#include "Command.h"

#include <QtCore/QVector>
#include <QtCore/QMap>

#include "model/Face.h"
#include "model/Edge.h"

#include "maths/Vec3.h"

class ExtrudeCommand : public Command
{
    Q_OBJECT

public:
    enum Type { Edges, Faces };

    ExtrudeCommand(Type type, Model *model, QObject *parent = 0);

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
    setEdges(const QVector<Edge> &v);

    void
    transform(const Matrix &m);

private:
    void
    createGeometry();

    Type type;

    QVector<Edge> edges;
    QMap<int, Face> modifiedFaces;
    QMap<int, Vec3> modifiedVertices;
    QVector<int> addedFaces;
    QMap<int, int> duplicated;
    QMap<int, bool> verticesSelected;

    Matrix translation;

    bool valid;
};

#endif // EXTRUDECOMMAND_H
