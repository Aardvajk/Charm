#ifndef SUPPORTPROVIDER_H
#define SUPPORTPROVIDER_H

#include "maths/Maths.h"

class SupportProvider
{
public:
    virtual ~SupportProvider();

    virtual Vec3 support(const Vec3 &normal) const;
};

#endif // SUPPORTPROVIDER_H
