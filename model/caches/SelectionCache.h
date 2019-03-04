#ifndef SELECTIONCACHE_H
#define SELECTIONCACHE_H

#include <QtCore/QVector>
#include <QtCore/QSet>

#include "maths/Vec3.h"

class Model;

class SelectionCache
{
public:
    SelectionCache(const Model *model);

    void
    invalidate();

    const QVector<int>&
    vertices() const;

    const QVector<int>&
    faces() const;

    const QVector<int>&
    joints() const;

    const QVector<int>&
    verticesInSelection() const;

    const QVector<int>&
    jointsInSelection() const;

    Vec3
    selectionCentre() const;

    bool
    anySelection() const;

private:
    void
    refresh() const;

    void
    addJointChildren(int index, QSet<int> &joints) const;

    void
    calculateAverageCentre(const QSet<int> &allVerts) const;

    const Model *model;

    mutable bool valid;

    mutable QVector<int> selectedVerts;
    mutable QVector<int> selectedFaces;
    mutable QVector<int> selectedJoints;

    mutable QVector<int> selectedAllVerts;
    mutable QVector<int> selectedAllJoints;

    mutable Vec3 centre;
};

#endif // SELECTIONCACHE_H
