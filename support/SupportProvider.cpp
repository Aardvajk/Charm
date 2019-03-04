#include "SupportProvider.h"

SupportProvider::~SupportProvider()
{
}

Vec3 SupportProvider::support(const Vec3 &normal) const
{
//    return normal * 10;

    Vec3 r(0, 0, 0);

    if(normal.x < 0) r.x = -10;
    else r.x = 10;

    if(normal.y < 0) r.y = -10;
    else r.y = 10;

    if(normal.z < 0) r.z = -10;
    else r.z = 10;

    return r + (normal * 5);
}
