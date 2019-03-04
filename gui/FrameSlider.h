#ifndef FRAMESLIDER_H
#define FRAMESLIDER_H

#include <QtWidgets/QWidget>

class VariantAnimation;

class FrameSlider : public QWidget
{
    Q_OBJECT

public:
    FrameSlider(QWidget *parent = 0);

    void
    setRange(float minimum, float maximum);

    float
    minimum() const { return min; }

    float
    maximum() const { return max; }

    void
    setValue(float value);

    float
    value() const { return val; }

    bool
    snapMode() const { return snapping; }

    void
    setKeyPositions(const QVector<float> &values);

    void
    setEventPositions(const QVector<float> &values);

    int
    currentKey() const { return currKey; }

    void
    setCurrentKey(int index);

    int
    currentEvent() const { return currEvent; }

    void
    setCurrentEvent(int index);

    void
    setBlockUserInteraction(bool state);

signals:
    void
    valueChanged(float value);

    void
    currentKeyPressed(int index);

    void
    currentKeyChanged(int index);

    void
    currentEventChanged(int index);

    void
    currentEventDoubleClicked(int index);

public slots:
    void
    setSnapMode(bool state);

protected:
    virtual
    void
    paintEvent(QPaintEvent *event);

    virtual
    void
    resizeEvent(QResizeEvent *event);

    virtual
    void
    mousePressEvent(QMouseEvent *event);

    virtual
    void
    mouseMoveEvent(QMouseEvent *event);

    virtual
    void
    mouseDoubleClickEvent(QMouseEvent *event);

    bool
    event(QEvent *event);

private:
    void
    paintScale(QPainter &painter);

    void
    paintKey(QPainter &painter, int position, bool selected);

    void
    paintEvent(QPainter &painter, int position, bool selected);

    void
    paintMarker(QPainter &painter, int position);

    void
    calculateSizes();

    int
    valueToScreen(float value) const;

    float
    screenToValue(int x) const;

    float
    constrainValue(float value) const;

    void
    updateValue(float value);

    void
    updateCurrKey(int value);

    void
    updateCurrEvent(int value);

    float
    snap(float value) const;

    QVector<float> keys;
    QVector<float> events;

    QRect frameRect;
    QVector<QRect> keyRects;

    float min;
    float max;

    float val;

    int currKey;
    int currEvent;

    VariantAnimation *anim;

    bool snapping;
    bool blockUser;
};

#endif // FRAMESLIDER_H
