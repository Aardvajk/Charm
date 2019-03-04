#ifndef PANELBUTTON_H
#define PANELBUTTON_H

#include <QtCore/QVariant>

#include <QtWidgets/QWidget>

class VariantAnimation;

class PanelButton : public QWidget
{
    Q_OBJECT

public:
    PanelButton(const QString &text, QWidget *parent = 0);

    virtual
    QSize
    sizeHint() const;

    void
    setCheckable(bool state);

    bool
    down() const { return toggleState; }

    QString
    text() const { return buttonText; }

    void
    setFrame(bool state);

    void
    stopAnimation();

    void
    clearAnimation();

    QVariant
    data() const { return buttonData; }

    void
    setData(const QVariant &data);

    void
    setText(const QString &text);

    void
    setPixmap(const QPixmap &pixmap);

    void
    setPixmap(const QPixmap &enabled, const QPixmap &disabled);

    void
    setComboStyle(bool state);

signals:
    void
    pressed();

    void
    toggled(bool state);

    void
    clicked();

public slots:
    void
    setDown(bool state);

    void
    toggle();

protected:
    virtual
    void
    paintEvent(QPaintEvent *event);

    virtual
    void
    mousePressEvent(QMouseEvent *event);

    virtual
    void
    mouseMoveEvent(QMouseEvent *event);

    virtual
    void
    mouseReleaseEvent(QMouseEvent *event);

    bool
    event(QEvent *event);

protected slots:
    void
    animValueChanged(const QVariant &value);

private:
    QString buttonText;
    QPixmap buttonPixmap;
    QPixmap buttonPixmapDis;
    VariantAnimation *anim;

    bool downState;
    bool inBounds;

    bool toggleState;
    bool checkableState;

    bool hasFrame;
    bool comboStyle;

    QVariant buttonData;
};

#endif // PANELBUTTON_H
