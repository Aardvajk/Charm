#ifndef JOINTMATRIXCACHE_H
#define JOINTMATRIXCACHE_H

#include <QtCore/QVector>

#include "maths/Matrix.h"

class Model;
class KeyFrame;

class JointMatrixCache
{
public:
    JointMatrixCache(const Model *model);

    void
    invalidate();

    Matrix
    matrix(int joint, const KeyFrame &key, bool useKey) const;

private:
    void
    refresh(const KeyFrame &key, bool useKey) const;

    Matrix
    buildMatrix(int index, const KeyFrame &key, bool useKey) const;

    const Model *model;
    mutable bool valid;
    mutable QVector<Matrix> data;
};

#endif // JOINTMATRIXCACHE_H
