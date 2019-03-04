#include "ToolButton.h"

#include "common/VariantAnimation.h"

#include "style/Style.h"

#include <QtCore/QEvent>

#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QMenu>

ToolButton::ToolButton(QWidget *parent)
    : QWidget(parent)
{
    construct();
}

ToolButton::ToolButton(const QPixmap &pixmap, QWidget *parent)
    : QWidget(parent)
{
    construct();
    addImage(pixmap);
}

QSize
ToolButton::sizeHint() const
{
    int iconWidth = 0;
    if(pixIndex >= 0)
    {
        iconWidth = 15;
    }

    int textWidth = 0;

    if(!buttonText.isEmpty())
    {
        QFontMetrics fm(font());
        textWidth = fm.width(buttonText);
    }

    return QSize((iconWidth + textWidth) + 8, 23);
}

void
ToolButton::addImage(const QPixmap &pixmap)
{
    if(pix.isEmpty())
    {
        pixIndex = 0;

        updateGeometry();
        update();
    }

    pix.append(pixmap);
}

void
ToolButton::setImageIndex(int index)
{
    if(index >= 0 && index < pix.count() && index != pixIndex)
    {
        pixIndex = index;

        updateGeometry();
        update();
    }
}

void
ToolButton::setText(const QString &text)
{
    if(text != buttonText)
    {
        buttonText = text;

        updateGeometry();
        update();
    }
}

void
ToolButton::setData(const QVariant &value)
{
    itemData = value;
}

void
ToolButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if(!down || !inBounds)
    {
        float alpha = anim->currentValue().toFloat();

        QColor color(255, 255, 255, static_cast<int>(96 * alpha));

        painter.setPen(color);
        painter.setBrush(color);
    }
    else
    {
        painter.setPen(QColor(0, 0, 0, 64));
        painter.setBrush(Style::baseColor().darker(120));
    }

    painter.drawRect(rect().adjusted(0, 0, -1, -2));

    if(pixIndex >= 0 && pixIndex < pix.count())
    {
        QRect pixRect(QPoint(0, 0), pix[pixIndex].size());
        QRect subRect = rect();

        pixRect.moveCenter(subRect.center());

        painter.drawPixmap(pixRect, pix[pixIndex]);
    }
    else if(!buttonText.isEmpty())
    {
        painter.setPen(Qt::black);
        painter.drawText(rect(), Qt::AlignVCenter | Qt::AlignHCenter, buttonText);
    }
}

void
ToolButton::mousePressEvent(QMouseEvent *event)
{
    down = true;
    inBounds = true;

    update();

    emit pressed();
}

void
ToolButton::mouseMoveEvent(QMouseEvent *event)
{
    inBounds = (rect().contains(event->pos()));
    update();
}

void
ToolButton::mouseReleaseEvent(QMouseEvent *event)
{
    down = false;
    update();

    if(inBounds)
    {
        emit clicked();
    }

    if(!QRect(mapToGlobal(QPoint(0, 0)), size()).contains(event->globalPos()))
    {
        clearAnimation();
    }
}

bool
ToolButton::event(QEvent *event)
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
ToolButton::animValueChanged(const QVariant &value)
{
    update();
}

void
ToolButton::construct()
{
    pixIndex = -1;

    down = false;
    inBounds = false;

    setMinimumSize(23, 23);

    anim = new VariantAnimation(this);
    anim->setStartValue(0.0f);

    connect(anim, SIGNAL(valueChanged(QVariant)), SLOT(animValueChanged(QVariant)));

    setAttribute(Qt::WA_Hover);
}

void
ToolButton::clearAnimation()
{
    down = false;

    anim->stop();

    anim->setStartValue(anim->currentValue().toFloat());
    anim->setEndValue(0.0f);
    anim->setDuration(400);

    anim->start();

    update();
}
