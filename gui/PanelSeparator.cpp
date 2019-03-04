#include "PanelSeparator.h"

#include <QtGui/QPainter>

#include "style/Style.h"

PanelSeparator::PanelSeparator(QWidget *parent)
    : QWidget(parent)
{
}

QSize
PanelSeparator::sizeHint() const
{
    return QSize(0, 16);
}

void
PanelSeparator::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QColor color = Style::baseColor();

    int y = rect().height() / 2;

    painter.setPen(color.darker(130));
    painter.drawLine(0, y, rect().width(), y);

    painter.setPen(color);
    painter.drawLine(0, y + 1, rect().width(), y + 1);
}
