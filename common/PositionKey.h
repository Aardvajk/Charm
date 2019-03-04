#ifndef POSITIONKEY_H
#define POSITIONKEY_H

#include "maths/Vec3.h"

class PositionKey
{
public:
    PositionKey(const Vec3 &vec);

    bool
    operator<(const PositionKey &p) const;

private:
    int x;
    int y;
    int z;
};

#endif // POSITIONKEY_H
