#ifndef GROUPCACHE_H
#define GROUPCACHE_H

#include <QtCore/QMap>

#include "maths/Matrix.h"

class Model;

class GroupCache
{
public:
    GroupCache(const Model *model);

    void
    invalidate();

    int
    index(int id) const;

    bool
    containsId(int id) const;

    int
    count(int id) const;

private:
    void
    update() const;

    const Model *model;
    mutable bool valid;
    mutable QMap<int, int> idMap;
    mutable QMap<int, int> countMap;
};

#endif // GROUPCACHE_H
