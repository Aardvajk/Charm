#include "ToolBar.h"

#include "style/Style.h"

#include <QtWidgets/QHBoxLayout>
#include <QtGui/QPainter>
#include <QtGui/QLinearGradient>

ToolBar::ToolBar(QWidget *parent)
    : QWidget(parent)
{
    layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    setMaximumHeight(23);
    setMinimumHeight(24);
}

void
ToolBar::addWidget(QWidget *widget)
{
    layout->addWidget(widget);
}

void
ToolBar::addStretch()
{
    layout->addStretch();
}

void
ToolBar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QColor color = Style::baseColor();

    QLinearGradient g(rect().topLeft(), rect().bottomLeft());
    g.setColorAt(0, color.darker(110));
    g.setColorAt(1, color.darker(140));

    painter.fillRect(rect(), g);

    color = QColor(0, 0, 0, 200);
    painter.setPen(color);

    painter.drawLine(rect().bottomLeft(), rect().bottomRight());
}

void
ToolBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit doubleClicked();
}
