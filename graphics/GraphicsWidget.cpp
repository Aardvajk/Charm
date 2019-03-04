#include "GraphicsWidget.h"
#include "GraphicsDevice.h"

#include <QtGui/QPainter>

GraphicsWidget::GraphicsWidget(GraphicsDevice *graphics, QWidget *parent)
    : QWidget(parent),
      graphics(graphics)
{
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);

    setMinimumSize(64, 64);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    graphics->registerWidget(this);
}

GraphicsWidget::~GraphicsWidget()
{
    graphics->unregisterWidget(this);
}

void
GraphicsWidget::paintEvent(QPaintEvent *event)
{
    graphics->begin(this, D3DCOLOR_XRGB(0, 0, 0));
    graphics->end(this);
}
