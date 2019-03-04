#include "FrameSlider.h"

#include "style/Style.h"

#include "common/VariantAnimation.h"
#include "common/Color.h"

#include "maths/Maths.h"

#include <QtGui/QPainter>
#include <QtGui/QLinearGradient>
#include <QtGui/QMouseEvent>

FrameSlider::FrameSlider(QWidget *parent)
    : QWidget(parent),
      min(1),
      max(18),
      val(1),
      currKey(-1),
      currEvent(-1),
      snapping(true),
      blockUser(false)
{
    anim = new VariantAnimation(this);
    connect(anim, SIGNAL(valueChanged(QVariant)), SLOT(update()));

    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    setMinimumWidth(128);

    setAttribute(Qt::WA_Hover);

    calculateSizes();
}

void
FrameSlider::setRange(float minimum, float maximum)
{
    if(min < max)
    {
        min = minimum;
        max = maximum;

        updateValue(val);
        updateCurrKey(-1);
        updateCurrEvent(-1);

        update();
    }
}

void
FrameSlider::setValue(float value)
{
    updateValue(value);
    updateCurrKey(-1);

    update();
}

void
FrameSlider::setKeyPositions(const QVector<float> &values)
{
    keys = values;
    if(currKey >= values.count())
    {
        updateCurrKey(-1);
    }

    update();
}

void
FrameSlider::setEventPositions(const QVector<float> &values)
{
    events = values;
    if(currEvent >= values.count())
    {
        updateCurrEvent(-1);
    }

    update();
}

void
FrameSlider::setCurrentKey(int index)
{
    if(index < keys.count() && index != currKey)
    {
        updateCurrKey(index);
        update();
    }
}

void
FrameSlider::setCurrentEvent(int index)
{
    if(index < events.count() && index != currEvent)
    {
        updateCurrEvent(index);
        update();
    }
}

void
FrameSlider::setBlockUserInteraction(bool state)
{
    blockUser = state;
}

void
FrameSlider::setSnapMode(bool state)
{
    snapping = state;
}

void
FrameSlider::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if(isEnabled())
    {
        int a = 100.0f * anim->currentValue().toFloat();

        painter.setPen(QColor(0, 0, 0, a));
        painter.setBrush(QColor(0, 0, 0, a));

        painter.save();
        painter.translate(2, 2);
        painter.drawRect(frameRect.adjusted(1, 1, -1, -2));
        painter.restore();
    }

    painter.setPen(Style::baseColor().darker(240));
    painter.setBrush(Style::baseColor().darker(120));

    painter.drawRect(frameRect.adjusted(0, 0, 0, -1));

    if(isEnabled())
    {
        paintScale(painter);

        for(int i = 0; i < events.count(); ++i)
        {
            paintEvent(painter, valueToScreen(events[i]), i == currEvent);
        }

        for(int i = 0; i < keys.count(); ++i)
        {
            paintKey(painter, valueToScreen(keys[i]), i == currKey);
        }

        paintMarker(painter, valueToScreen(val));
    }
}

void
FrameSlider::resizeEvent(QResizeEvent *event)
{
    calculateSizes();
}

void
FrameSlider::mousePressEvent(QMouseEvent *event)
{
    if(blockUser)
    {
        return;
    }

    bool found = false;

    int gotEvent = -1;
    int gotKey = -1;

    if(event->pos().y() > frameRect.top() && event->pos().y() < frameRect.bottom())
    {
        for(int i = events.count() - 1; i >= 0; --i)
        {
            int pos = valueToScreen(events[i]);

            if(abs(pos - event->pos().x()) <= 11)
            {
                gotEvent = i;
                break;
            }
        }
    }

    if(event->pos().y() < frameRect.top() + (frameRect.height() / 2))
    {
        for(int i = keys.count() - 1; i >= 0; --i)
        {
            int pos = valueToScreen(keys[i]);

            if(abs(pos - event->pos().x()) <= 5)
            {
                gotEvent = -1;
                gotKey = i;
                break;
            }
        }
    }

    if(gotEvent != -1)
    {
        updateCurrEvent(gotEvent);
        updateCurrKey(-1);
        updateValue(events[gotEvent]);

        found = true;
    }

    if(gotKey != -1)
    {
        updateCurrKey(gotKey);
        updateCurrEvent(-1);
        updateValue(keys[gotKey]);

        found = true;
        emit currentKeyPressed(gotKey);
    }

    if(!found)
    {
        updateCurrKey(-1);
        updateCurrEvent(-1);
        updateValue(snap(screenToValue(event->pos().x())));
    }

    update();
}

void
FrameSlider::mouseMoveEvent(QMouseEvent *event)
{
    if(blockUser)
    {
        return;
    }

    updateValue(snap(screenToValue(event->pos().x())));
    update();
}

void
FrameSlider::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(blockUser)
    {
        return;
    }

    if(currEvent >= 0)
    {
        emit currentEventDoubleClicked(currEvent);
    }
}

bool
FrameSlider::event(QEvent *event)
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

    return QWidget::event(event);
}

void
FrameSlider::paintScale(QPainter &painter)
{
    QColor color = Style::baseColor().darker(120);

    bool alternate = false;

    for(float a = min; a < max; ++a)
    {
        int pa = valueToScreen(a);
        int pb = valueToScreen(a + 1);

        painter.fillRect(QRect(pa + 1, frameRect.top() + 1, (pb - pa) - 1, frameRect.bottom() - 5), alternate ? color.darker(105) : color);

        alternate = !alternate;
    }
}

void
FrameSlider::paintKey(QPainter &painter, int position, bool selected)
{
    QRect r(position - 5, frameRect.top() - 4, 11, 9);

    QPoint point = QPoint(position, r.top() + 13);

    QVector<QPoint> pts = QVector<QPoint>() << r.topLeft() << r.topRight() << r.bottomRight() << point << r.bottomLeft();

    int a = 100.0f * anim->currentValue().toFloat();

    painter.setPen(QColor(0, 0, 0, a));
    painter.setBrush(QColor(0, 0, 0, a));

    painter.save();
    painter.translate(1, 1);
    painter.drawPolygon(pts);
    painter.restore();

    QColor color = interpolate(Style::baseColor().darker(120), selected ? Style::baseColor() : Style::baseColor().darker(120), anim->currentValue().toFloat());
    QColor border = Style::baseColor().darker(240);

    painter.setPen(border);
    painter.setBrush(color);

    painter.drawPolygon(pts);

    painter.setPen(setAlpha(border, 70));

    painter.drawLine(r.bottomLeft() + QPoint(0, 1), point + QPoint(-1, 0));
    painter.drawLine(r.bottomLeft() + QPoint(1, 0), point + QPoint(-1, -2));

    painter.drawLine(r.bottomRight() + QPoint(0, 1), point + QPoint(1, 0));
    painter.drawLine(r.bottomRight() + QPoint(-1, 0), point + QPoint(0, -1));
}

void
FrameSlider::paintEvent(QPainter &painter, int position, bool selected)
{
    float w = 9;
    float h = (frameRect.height() / 2) - 2;

    QPoint point = QPoint(position, frameRect.top() + h + 2);

    QVector<QPoint> pts = QVector<QPoint>() << point + QPoint(0, -h) << point + QPoint(w, 0) << point + QPoint(0, h) << point + QPoint(-w, 0);

    int a = 100.0f * anim->currentValue().toFloat();

    painter.setPen(QColor(0, 0, 0, a));
    painter.setBrush(QColor(0, 0, 0, a));

    painter.save();
    painter.translate(1, 1);
    painter.drawPolygon(pts);
    painter.restore();

    QColor color = interpolate(Style::baseColor().darker(120), selected ? Style::baseColor() : Style::baseColor().darker(120), anim->currentValue().toFloat());
    QColor border = Style::baseColor().darker(240);

    painter.setPen(border);
    painter.setBrush(color);

    painter.drawPolygon(pts);

//    painter.setPen(setAlpha(border, 70));

//    painter.drawLine(r.bottomLeft() + QPoint(0, 1), point + QPoint(-1, 0));
//    painter.drawLine(r.bottomLeft() + QPoint(1, 0), point + QPoint(-1, -2));

//    painter.drawLine(r.bottomRight() + QPoint(0, 1), point + QPoint(1, 0));
//    painter.drawLine(r.bottomRight() + QPoint(-1, 0), point + QPoint(0, -1));
}

void
FrameSlider::paintMarker(QPainter &painter, int position)
{
    QRect r(position - 5, frameRect.bottom() - 6, 11, 10);

    QPoint point = QPoint(position, frameRect.bottom() - 11);

    QVector<QPoint> pts = QVector<QPoint>() << r.topLeft() << point << r.topRight() << r.bottomRight() << r.bottomLeft();

    int a = 100.0f * anim->currentValue().toFloat();

    painter.setPen(QColor(0, 0, 0, a));
    painter.setBrush(QColor(0, 0, 0, a));

    painter.save();
    painter.translate(1, 1);
    painter.drawPolygon(pts);
    painter.restore();

    QColor color = interpolate(Style::baseColor().darker(120), Style::baseColor(), anim->currentValue().toFloat());
    QColor border = Style::baseColor().darker(240);

    painter.setPen(border);
    painter.setBrush(color);

    painter.drawPolygon(pts);

    painter.setPen(setAlpha(border, 70));

    painter.drawLine(r.topLeft() + QPoint(0, -1), point + QPoint(-1, 0));
    painter.drawLine(r.topLeft() + QPoint(1, 0), point + QPoint(-1, 2));

    painter.drawLine(point + QPoint(1, 0), r.topRight() + QPoint(0, -1));
    painter.drawLine(point + QPoint(0, 1), r.topRight() + QPoint(-1, 0));
}

void
FrameSlider::calculateSizes()
{
    frameRect = QRect(rect().left() + 11, rect().top() + 4, rect().width() - 22, rect().height() - 8);
}

int
FrameSlider::valueToScreen(float value) const
{
    float range = max - min;

    int pos = 0;
    if(range)
    {
        float width = frameRect.width();
        float norm = value - min;

        float factor = norm / range;

        pos = width * factor;
    }

    return frameRect.left() + pos;
}

float
FrameSlider::screenToValue(int x) const
{
    float range = max - min;

    if(range)
    {
        float width = frameRect.width();
        float norm = x - frameRect.left();

        float factor = norm / width;

        return min + (range * factor);
    }

    return min;
}

float
FrameSlider::constrainValue(float value) const
{
    if(value < min) value = min;
    if(value > max) value = max;

    return value;
}

void
FrameSlider::updateValue(float value)
{
    value = constrainValue(value);

    if(val != value)
    {
        val = value;
        emit valueChanged(val);
    }
}

void
FrameSlider::updateCurrKey(int value)
{
    if(currKey != value)
    {
        currKey = value;
        emit currentKeyChanged(currKey);
    }
}

void
FrameSlider::updateCurrEvent(int value)
{
    if(currEvent != value)
    {
        currEvent = value;
        emit currentEventChanged(currEvent);
    }
}

float
FrameSlider::snap(float value) const
{
    return snapping ? round(value) : value;
}
