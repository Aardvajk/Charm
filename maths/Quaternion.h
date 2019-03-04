#ifndef QUATERNION_H
#define QUATERNION_H

#include "maths/Maths.h"

Quaternion
quaternionRotationTo(const Vec3 &from, const Vec3 &to);

Quaternion
quaternionAxisAngle(const Vec3 &axis, float angle);

Quaternion
interpolateQuaternions(const Quaternion &a, const Quaternion &b, float t);

#endif // QUATERNION_H
