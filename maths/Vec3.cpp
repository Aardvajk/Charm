#include "Vec3.h"

float
dotVectors(const Vec3 &a, const Vec3 &b)
{
    return D3DXVec3Dot(&a, &b);
}

float
vectorLength(const Vec3 &v)
{
    return D3DXVec3Length(&v);
}

Vec3
normalizeVector(const Vec3 &v)
{
    Vec3 r;
    D3DXVec3Normalize(&r, &v);

    return r;
}

Vec3
crossVectors(const Vec3 &a, const Vec3 &b)
{
    Vec3 r;
    D3DXVec3Cross(&r, &a, &b);

    return r;
}

Vec3
transformCoord(const Vec3 &v, const Matrix &m)
{
    Vec3 r;
    D3DXVec3TransformCoord(&r, &v, &m);

    return r;
}

Vec3
transformNormal(const Vec3 &v, const Matrix &m)
{
    Vec3 r;
    D3DXVec3TransformNormal(&r, &v, &m);

    return r;
}

Vec3
interpolateVectors(const Vec3 &a, const Vec3 &b, float t)
{
    return (a * t) + (b * (1.0f - t));
}

Vec3
projectPointToPlane(const Vec3 &p, const Vec3 &o, const Vec3 &n)
{
    return p - (n * dotVectors(p - o, n));
}

Vec3
mulVectors(const Vec3 &a, const Vec3 &b)
{
    return Vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}

Vec3
absVector(const Vec3 &v)
{
    return Vec3(fabs(v.x), fabs(v.y), fabs(v.z));
}

int
windingOrder(const Vec3 &a, const Vec3 &b, const Vec3 &c)
{
    Vec3 x = b - a, y = c - b;
    return(dotVectors(Vec3(-x.y, x.x, 0), y) < 0 ? -1 : 1);
}

Vec3
degreesToRadians(const Vec3 &v)
{
    return Vec3(degreeToRadian(v.x), degreeToRadian(v.y), degreeToRadian(v.z));
}

Vec3
interpolate(const Vec3 &a, const Vec3 &b, float t)
{
    return (b * t) + (a * (1.0f - t));
}
