#ifndef STYLE_H
#define STYLE_H

#include <QtWidgets/QProxyStyle>

class Style : public QProxyStyle
{
public:
    Style(QStyle *style) : QProxyStyle(style) { }

    virtual
    void
    drawControl(ControlElement element, const QStyleOption *opt, QPainter *p, const QWidget *w) const;

    static
    QColor
    baseColor();
};

#endif // STYLE_H
