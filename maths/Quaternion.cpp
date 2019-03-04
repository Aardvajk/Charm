#include "Quaternion.h"

#include "maths/Vec3.h"

Quaternion
quaternionRotationTo(const Vec3 &from, const Vec3 &to)
{
    Quaternion q;

    Vec3 v0 = normalizeVector(from);
    Vec3 v1 = normalizeVector(to);

    float d = dotVectors(v0, v1);

    if(d >= 1.0f)
    {
        return Quaternion(0, 0, 0, 0);
    }

    if(d < (1e-6f - 1.0f))
    {
        Vec3 axis = crossVectors(Vec3(1, 0, 0), from);
        if(dotVectors(axis,axis) == 0)
        {
            axis = crossVectors(Vec3(0, 1, 0), from);
        }

        axis = normalizeVector(axis);

        float ang = pi();
        D3DXQuaternionToAxisAngle(&q, &axis, &ang);
    }
    else
    {
        float s = sqrt((1 + d) * 2);
        float invs = 1 / s;

        Vec3 c = crossVectors(v0, v1);

        q.x = c.x * invs;
        q.y = c.y * invs;
        q.z = c.z * invs;
        q.w = s * 0.5f;

        D3DXQuaternionNormalize(&q, &q);
    }

    return q;
}

Quaternion
quaternionAxisAngle(const Vec3 &axis, float angle)
{
    Quaternion q;
    D3DXQuaternionRotationAxis(&q, &axis, angle);

    return q;
}

Quaternion
interpolateQuaternions(const Quaternion &a, const Quaternion &b, float t)
{
    Quaternion c;
    D3DXQuaternionSlerp(&c, &a, &b, t);

    return c;
}
