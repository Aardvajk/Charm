#ifndef VEC3_H
#define VEC3_H

#include "maths/Maths.h"

float
dotVectors(const Vec3 &a, const Vec3 &b);

float
vectorLength(const Vec3 &v);

Vec3
normalizeVector(const Vec3 &v);

Vec3
crossVectors(const Vec3 &a, const Vec3 &b);

Vec3
transformCoord(const Vec3 &v, const Matrix &m);

Vec3
transformNormal(const Vec3 &v, const Matrix &m);

Vec3
interpolateVectors(const Vec3 &a, const Vec3 &b, float t);

Vec3
projectPointToPlane(const Vec3 &p, const Vec3 &o, const Vec3 &n);

Vec3
mulVectors(const Vec3 &a, const Vec3 &b);

Vec3
absVector(const Vec3 &v);

int
windingOrder(const Vec3 &a, const Vec3 &b, const Vec3 &c);

Vec3
degreesToRadians(const Vec3 &v);

Vec3
interpolate(const Vec3 &a, const Vec3 &b, float t);

#endif // VEC3_H
