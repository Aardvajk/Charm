#include "ToolPanelButton.h"

#include "common/VariantAnimation.h"

#include "tools/Tool.h"

#include <QtCore/QEvent>

#include <QtGui/QPainter>
#include <QtGui/QLinearGradient>

ToolPanelButton::ToolPanelButton(Tool *tool, QWidget *parent)
    : QWidget(parent),
      instance(tool),
      down(false)
{
    setMinimumSize(64, 24);
    setMaximumSize(64, 64);

    setAttribute(Qt::WA_Hover);

    anim = new VariantAnimation(this);
    connect(anim, SIGNAL(valueChanged(QVariant)), SLOT(animValueChanged(QVariant)));

    connect(tool, SIGNAL(enabled(bool)), SLOT(setVisible(bool)));
    connect(tool, SIGNAL(selected()), SLOT(select()));
}

QSize
ToolPanelButton::sizeHint() const
{
    return maximumSize();
}

void
ToolPanelButton::setDown(bool state)
{
    if(down != state)
    {
        down = state;
        update();
    }
}

void
ToolPanelButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect colorRect = rect().adjusted(0, 0, 0, -1);

    if(!down)
    {
        float alpha = anim->currentValue().toFloat();

        QColor color(200, 220, 255, static_cast<int>(180 * alpha));
        painter.fillRect(rect(), color);
    }
    else
    {
        QColor color(100, 200, 255, 200);

        QLinearGradient g(colorRect.topLeft(), colorRect.bottomLeft());
        g.setColorAt(0, color.lighter(140));
        g.setColorAt(0.6f, color.darker(120));
        g.setColorAt(1, color.darker(120));

        painter.fillRect(colorRect, g);

        painter.setPen(color.darker(120));
        painter.drawLine(colorRect.topLeft(), colorRect.topRight());

        painter.setPen(color.darker(140));
        painter.drawLine(colorRect.bottomLeft(), colorRect.bottomRight());

        painter.setPen(QColor(255, 255, 255, 230));
        painter.drawLine(rect().bottomLeft(), rect().bottomRight());
    }

    if(!instance->image().isNull())
    {
        int dim = rect().height() - 32;

        QRect pixRect(0, 0, dim, dim);
        pixRect.moveCenter(rect().adjusted(0, 0, 0, -12).center());

        painter.drawPixmap(pixRect, instance->image().scaledToHeight(dim, Qt::SmoothTransformation));
    }

    QRect textRect(0, rect().height() - 18, 64, 10);

    painter.setPen(Qt::black);
    painter.drawText(textRect, Qt::AlignHCenter | Qt::AlignVCenter, instance->name());
}

void
ToolPanelButton::mousePressEvent(QMouseEvent *event)
{
    select();
}

bool
ToolPanelButton::event(QEvent *event)
{
    if(event->type() == QEvent::HoverEnter)
    {
        anim->stop();

        anim->setStartValue(anim->currentValue().toFloat());
        anim->setEndValue(1.0f);
        anim->setDuration(200);

        anim->start();
    }
    else if(event->type() == QEvent::HoverLeave)
    {
        anim->stop();

        anim->setStartValue(anim->currentValue().toFloat());
        anim->setEndValue(0.0f);
        anim->setDuration(400);

        anim->start();
    }

    return QWidget::event(event);
}

void
ToolPanelButton::select()
{
    if(!down)
    {
        emit pressed();

        down = true;
        update();
    }
}

void
ToolPanelButton::animValueChanged(const QVariant &value)
{
    update();
}
