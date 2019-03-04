#include "Axis.h"

QString
Axis::toString(Type axis)
{
    switch(axis)
    {
        case X   : return "X";
        case Y   : return "Y";
        case Z   : return "Z";
        case None: return "None";
    }

    return QString();
}

Vec3
Axis::vector(Type axis)
{
    switch(axis)
    {
        case X   : return Vec3(-1, 0, 0);
        case Y   : return Vec3(0, 1, 0);
        case Z   : return Vec3(0, 0, 1);
        case None: return Vec3();
    }

    return Vec3();
}
