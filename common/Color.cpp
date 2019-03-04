#include "Color.h"

#include "maths/Maths.h"

QColor
setAlpha(const QColor &color, int alpha)
{
    QColor c = color;
    c.setAlpha(alpha);

    return c;
}

QColor
setAlpha(const QColor &color, float alpha)
{
    return setAlpha(color, static_cast<int>(255.0f * alpha));
}

D3DCOLOR
toD3dColor(const QColor &color)
{
    return D3DCOLOR_XRGB(color.red(), color.green(), color.blue());
}

QColor
interpolate(const QColor &a, const QColor &b, float t)
{
    float red = interpolate(float(a.red()), float(b.red()), t);
    float green = interpolate(float(a.green()), float(b.green()), t);
    float blue = interpolate(float(a.blue()), float(b.blue()), t);

    return QColor(int(red), int(green), int(blue));
}

