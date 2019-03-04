#ifndef FACEREFERENCECACHE_H
#define FACEREFERENCECACHE_H

#include <QtCore/QVector>

class Model;

class FaceReferenceCache
{
public:
    FaceReferenceCache(const Model *model);

    void
    invalidate();

    QVector<int>
    faces(int vertex) const;

private:
    void
    update() const;

    const Model *model;

    mutable bool valid;
    mutable QVector<QVector<int> > data;
};

#endif // FACEREFERENCECACHE_H
