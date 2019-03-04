#ifndef MATHS_H
#define MATHS_H

#include <d3dx9.h>

#include <QtCore/QString>
#include <QtCore/QPointF>

#include "common/StoredItem.h"

typedef D3DXVECTOR3 Vec3;
typedef D3DXMATRIX Matrix;
typedef D3DXQUATERNION Quaternion;

typedef StoredItem<Vec3> StoredPosition;
typedef StoredItem<Quaternion> StoredRotation;

class QSize;

float
pi();

float
epsilon();

float
maxFloat();

float
degreeToRadian(float d);

float
interpolate(float a, float b, float t);

QSize
interpolate(const QSize &a, const QSize &b, float t);

QPointF
normalizedPointToScreenCoord(const Vec3 &p, const QSize &size);

float
distance(const QPointF &a, const QPointF &b);

float
round(float f);

QString
formatFloat(float f);

float
triangleArea(const Vec3 &a, const Vec3 &b, const Vec3 &c);

#endif // MATHS_H
