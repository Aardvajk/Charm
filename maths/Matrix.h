#ifndef MATRICES_H
#define MATRICES_H

#include <QtCore/QSize>

#include "maths/Maths.h"

Matrix
identityMatrix();

Matrix
perspectiveMatrix(const QSize &size, float fov);

Matrix
orthoMatrix(float width, float height);

Matrix
orthoMatrix(const QSize &size, float pixelsPerUnit);

Matrix
orthoMatrix(const Vec3 &pos, const QSize &size, float fov);

float
orthoUnit(const Vec3 &pos, const QSize &size, float fov);

Matrix
lookAtMatrix(const Vec3 &eye, const Vec3 &look, const Vec3 &up);

Matrix
rotationXMatrix(float angle);

Matrix
rotationYMatrix(float angle);

Matrix
rotationZMatrix(float angle);

Matrix
rotationMatrix(const Vec3 &v);

Matrix
rotationMatrixDegrees(const Vec3 &v);

Matrix
rotationAxisMatrix(const Vec3 &axis, float angle);

Matrix
translationMatrix(const Vec3 &v);

Matrix
scalingMatrix(float scale);

Matrix
scalingMatrix(float x, float y, float z);

Matrix
scalingMatrix(const Vec3 &v);

Matrix
inverseMatrix(const Matrix &m);

Matrix
quaternionMatrix(const Quaternion &q);

Matrix
filterMatrix(const Matrix &m, const Vec3 &scale, const Vec3 &translate);

#endif // MATRICES_H
