#include "Joint.h"

#include "maths/Vec3.h"

JointTransform
interpolateJointTransforms(const JointTransform &a, const JointTransform &b, float t)
{
    JointTransform c;
    c.rotation = interpolateQuaternions(a.rotation, b.rotation, t);
    c.translation = interpolate(a.translation, b.translation, t);

    return c;
}

