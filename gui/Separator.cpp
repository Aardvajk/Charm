#include "Separator.h"

#include <QtGui/QPainter>

#include "style/Style.h"

Separator::Separator(QWidget *parent)
    : QWidget(parent)
{
    setMinimumHeight(2);
    setMaximumHeight(2);
}

void
Separator::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setPen(Style::baseColor().darker(180));
    painter.drawLine(0, 0, rect().width(), 0);

    painter.setPen(Style::baseColor().darker(120));
    painter.drawLine(0, 1, rect().width(), 1);
}

