#ifndef SELECTCOMMAND_H
#define SELECTCOMMAND_H

#include "Command.h"

#include <QtCore/QVector>
#include <QtCore/QPair>

#include "maths/Matrix.h"

#include "common/VariantSet.h"

class QRectF;

class SelectCommand : public Command
{
public:
    SelectCommand(Model *model, QObject *parent = 0);

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
    select(const QRectF &rect, const Matrix &worldMatrix, const Matrix &transform, bool preserve, const VariantSet &elements, bool frontOnly, float jointRadius);

    void
    deselect(const QRectF &rect, const Matrix &worldMatrix, const Matrix &transform, const VariantSet &elements, bool frontOnly, float jointRadius);

    void
    selectAll();

    void
    selectNone();

    void
    selectInvert();

    void
    selectVertices(const QVector<int> &v);

    void
    selectFaces(const QVector<int> &v);

    void
    selectJoints(const QVector<int> &v);

    void
    setJointSelection(const QVector<int> &v);

private:
    bool
    triangleIntersects(int i, const QRectF &rect, const Matrix &transform, bool frontOnly) const;

    bool
    jointIntersects(int i, float jointRadius, const QRectF &rect, const Matrix &worldMatrix, const Matrix &transform) const;

    QVector<QPair<int, bool> > verts;
    QVector<QPair<int, bool> > faces;
    QVector<QPair<int, bool> > joints;
};

#endif // SELECTCOMMAND_H
