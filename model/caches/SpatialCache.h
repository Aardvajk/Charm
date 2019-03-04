#ifndef SPATIALCACHE_H
#define SPATIALCACHE_H

#include "common/PositionKey.h"

#include <QtCore/QMap>
#include <QtCore/QPair>

class Model;

class SpatialCache
{
public:
    SpatialCache(const Model *model);

    void
    invalidate();

    const QVector<int>&
    vertices(int vertex) const;

    bool
    selected(int vertex) const;

private:
    void
    refresh() const;

    const Model *model;
    mutable bool valid;
    mutable QMap<PositionKey, QPair<QVector<int>, bool> > data;
};

#endif // SPATIALCACHE_H
