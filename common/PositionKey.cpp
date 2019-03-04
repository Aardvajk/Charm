#include "PositionKey.h"

PositionKey::PositionKey(const Vec3 &vec)
{
    const float precision = 1000.0f;

    x = static_cast<int>(vec.x * precision);
    y = static_cast<int>(vec.y * precision);
    z = static_cast<int>(vec.z * precision);
}

bool
PositionKey::operator<(const PositionKey &p) const
{
    if(x < p.x)
    {
        return true;
    }
    else if(x > p.x)
    {
        return false;
    }

    if(y < p.y)
    {
        return true;
    }
    else if(y > p.y)
    {
        return false;
    }

    if(z < p.z)
    {
        return true;
    }

    return false;
}
