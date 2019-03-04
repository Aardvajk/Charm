#include "Maths.h"
#include "Vec3.h"

#include <math.h>
#include <float.h>

#include <QtCore/QSize>

float pi()
{
    return float(M_PI);
}

float
epsilon()
{
    return 0.01f;
}

float
maxFloat()
{
    return FLT_MAX;
}

float
degreeToRadian(float d)
{
    return (pi() / 180.0f) * d;
}

float
interpolate(float a, float b, float t)
{
    return (b * t) + (a * (1.0f - t));
}

QSize
interpolate(const QSize &a, const QSize &b, float t)
{
    float w = interpolate(static_cast<float>(a.width()), static_cast<float>(b.width()), t);
    float h = interpolate(static_cast<float>(a.height()), static_cast<float>(b.height()), t);

    return QSize(static_cast<int>(w), static_cast<int>(h));
}

QPointF
normalizedPointToScreenCoord(const Vec3 &p, const QSize &size)
{
    QPointF r;

    float x = (p.x + 1.0f) * 0.5f;
    float y = (-p.y + 1.0f) * 0.5f;

    r.setX(size.width() * x);
    r.setY(size.height() * y);

    return r;
}

float
distance(const QPointF &a, const QPointF &b)
{
    float x = a.x() - b.x();
    float y = a.y() - b.y();

    return sqrtf((x * x) + (y * y));
}

float
round(float f)
{
    float r = f - floor(f);
    return floor(f) + (r >= 0.5f ? 1.0f : 0.0f);
}

QString
formatFloat(float f)
{
    QString s = QString::number(f, 'f');

    while(s.contains('.'))
    {
        if(s.endsWith('0') || s.endsWith('.'))
        {
            s.chop(1);
        }
        else
        {
            break;
        }
    }

    return s;
}

float
triangleArea(const Vec3 &a, const Vec3 &b, const Vec3 &c)
{
    return vectorLength(crossVectors(a - b, a - c)) * 0.5f;
}
