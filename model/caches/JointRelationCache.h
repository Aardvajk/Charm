#ifndef JOINTRELATIONCACHE_H
#define JOINTRELATIONCACHE_H

#include <QtCore/QVector>

class Model;

class JointRelationCache
{
public:
    JointRelationCache(const Model *model);

    void
    invalidate();

    const QVector<int>&
    children(int index) const;

private:
    void
    refresh() const;

    const Model *model;
    mutable bool valid;
    mutable QVector<QVector<int> > data;
};

#endif // JOINTRELATIONCACHE_H
