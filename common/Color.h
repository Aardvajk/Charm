#ifndef COLOR_H
#define COLOR_H

#include <QtGui/QColor>
#include <d3d9.h>

QColor
setAlpha(const QColor &color, int alpha);

QColor
setAlpha(const QColor &color, float alpha);

D3DCOLOR
toD3dColor(const QColor &color);

QColor
interpolate(const QColor &a, const QColor &b, float t);

#endif // COLOR_H
