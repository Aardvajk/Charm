#include "Rotation.h"

#include "maths/Maths.h"

Vec3
Rotation::toVector(Rotation::Type type)
{
    switch(type)
    {
        case Front : return Vec3(0, 0, 0);
        case Back  : return Vec3(0, pi(), 0);
        case Left  : return Vec3(0, -pi() / 2, 0);
        case Right : return Vec3(0, pi() / 2, 0);
        case Top   : return Vec3(-pi() / 2, 0, 0);
        case Bottom: return Vec3(pi() / 2, 0, 0);

        default: return Vec3();
    }
}

QString
Rotation::toString(Rotation::Type type)
{
    switch(type)
    {
        case Front : return "Front";
        case Back  : return "Back";
        case Left  : return "Left";
        case Right : return "Right";
        case Top   : return "Top";
        case Bottom: return "Bottom";

        default: return "User";
    }
}
