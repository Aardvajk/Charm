#include "PaintListBox.h"

#include <QtGui/QPainter>

PaintListBox::PaintListBox(QWidget *parent)
    : PanelListBox(parent)
{
    setDrawSelection(true);
}

void
PaintListBox::paintItem(QPainter &painter, const QRect &rect, int index)
{
    QColor color = qvariant_cast<QColor>(items[index].data());

    painter.save();

    painter.setPen(index == currentItem() ? Qt::black : color);
    painter.setBrush(color);

    painter.drawRect(QRect(rect.left() + 2, rect.top() + 2, rect.width() - 5, rect.height() - 5));

    painter.restore();
}
