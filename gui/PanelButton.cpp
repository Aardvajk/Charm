#include "PanelButton.h"

#include "style/Style.h"

#include "maths/Maths.h"

#include "common/VariantAnimation.h"
#include "common/Color.h"

#include <QtCore/QEvent>

#include <QtGui/QPainter>
#include <QtGui/QLinearGradient>
#include <QtGui/QMouseEvent>

PanelButton::PanelButton(const QString &text, QWidget *parent)
    : QWidget(parent),
      buttonText(text),
      comboStyle(false)
{
    anim = new VariantAnimation(this);
    connect(anim, SIGNAL(valueChanged(QVariant)), SLOT(animValueChanged(QVariant)));

    setAttribute(Qt::WA_Hover);

    downState = false;
    inBounds = false;

    toggleState = true;

    hasFrame = true;

    checkableState = false;

    setMinimumHeight(22);
}

QSize
PanelButton::sizeHint() const
{
    QFontMetrics fm(font());

    if(comboStyle)
    {
        return QSize(fm.width(buttonText) + 26, 22);
    }
    else
    {
        return QSize(fm.width(buttonText) + 16, 22);
    }
}

void
PanelButton::setCheckable(bool state)
{
    checkableState = state;
    update();
}

void
PanelButton::setFrame(bool state)
{
    hasFrame = state;
    update();
}

void
PanelButton::stopAnimation()
{
    anim->stop();
}

void
PanelButton::clearAnimation()
{
    downState = false;

    anim->stop();

    anim->setStartValue(anim->currentValue().toFloat());
    anim->setEndValue(0.0f);
    anim->setDuration(1000);

    anim->start();
}

void
PanelButton::setData(const QVariant &data)
{
    buttonData = data;
}

void
PanelButton::setText(const QString &text)
{
    buttonText = text;

    updateGeometry();
    update();
}

void
PanelButton::setPixmap(const QPixmap &pixmap)
{
    buttonPixmap = pixmap;

    updateGeometry();
    update();
}

void
PanelButton::setPixmap(const QPixmap &enabled, const QPixmap &disabled)
{
    buttonPixmap = enabled;
    buttonPixmapDis = disabled;

    updateGeometry();
    update();
}

void
PanelButton::setComboStyle(bool state)
{
    comboStyle = state;

    updateGeometry();
    update();
}

void
PanelButton::setDown(bool state)
{
    if(toggleState != state)
    {
        toggleState = state;
        emit toggled(state);

        update();
    }
}

void
PanelButton::toggle()
{
    setDown(!toggleState);
}

void
PanelButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    float a = anim->currentValue().toFloat();

    if(!toggleState)
    {
        a *= 0.75f;
    }

    QColor color = interpolate(Style::baseColor().darker(120), QColor(180, 220, 255), a);

    if(toggleState && isEnabled())
    {
        QLinearGradient g(rect().topLeft(), rect().bottomLeft());
        g.setColorAt(0, color.lighter(120));
        g.setColorAt(0.5f, color.lighter(110));
        g.setColorAt(0.55f, color);
        g.setColorAt(1, color.darker(105));

        painter.fillRect(rect(), g);
    }
    else
    {
        painter.fillRect(rect(), color);
    }

    if(hasFrame)
    {
        painter.setBrush(Qt::NoBrush);
        painter.setPen(Style::baseColor().darker(200));

        painter.drawRect(rect().adjusted(0, 0, -1, -1));
    }

    if(downState && inBounds)
    {
        QColor frameColor = setAlpha(color.darker(200), 200);

        painter.setBrush(Qt::NoBrush);
        painter.setPen(frameColor);

        QRect r = rect().adjusted(0, 0, -1, -1);

        if(hasFrame)
        {
            r = r.adjusted(1, 1, -1, -1);
        }

        painter.drawRect(r);
    }

    if(buttonPixmap.isNull())
    {
        if(toggleState && isEnabled())
        {
            if(comboStyle)
            {
                painter.setPen(Qt::black);
                painter.drawText(rect().adjusted(6, 0, -18, 0), Qt::AlignVCenter | Qt::AlignLeft, buttonText);

                QRect r(rect().width() - 20, 0, 20, rect().height());
                QPixmap p(":/resources/images/dropdownarrow.png");

                QRect pr(QPoint(0, 0), p.size());
                pr.moveCenter(r.center());

                painter.drawPixmap(pr, p);
            }
            else
            {
                painter.setPen(Qt::black);
                painter.drawText(rect(), Qt::AlignVCenter | Qt::AlignHCenter, buttonText);
            }
        }
        else
        {
            painter.setPen(Qt::white);
            painter.drawText(rect().adjusted(1, 1, 0, 0), Qt::AlignVCenter | Qt::AlignHCenter, buttonText);

            painter.setPen(Qt::darkGray);
            painter.drawText(rect(), Qt::AlignVCenter | Qt::AlignHCenter, buttonText);
        }
    }
    else
    {
        QPixmap pix = isEnabled() ? buttonPixmap : buttonPixmapDis;

        QRect pixmapRect(QPoint(0, 0), pix.size());
        pixmapRect.moveCenter(rect().center());

        painter.drawPixmap(pixmapRect, pix);
    }
}

void
PanelButton::mousePressEvent(QMouseEvent *event)
{
    downState = true;
    inBounds = true;

    update();

    emit pressed();
}

void
PanelButton::mouseMoveEvent(QMouseEvent *event)
{
    inBounds = (rect().contains(event->pos()));
    update();
}

void
PanelButton::mouseReleaseEvent(QMouseEvent *event)
{
    downState = false;

    if(inBounds)
    {
        if(checkableState)
        {
            toggleState = !toggleState;
            emit toggled(toggleState);
        }
        else
        {
            emit clicked();
        }
    }

    update();
}

bool
PanelButton::event(QEvent *event)
{
    if(event->type() == QEvent::HoverEnter && isEnabled())
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
        anim->setDuration(1000);

        anim->start();
    }
    else if(event->type() == QEvent::EnabledChange)
    {
        if(!isEnabled())
        {
            clearAnimation();
        }
    }

    return QWidget::event(event);
}

void
PanelButton::animValueChanged(const QVariant &value)
{
    update();
}
