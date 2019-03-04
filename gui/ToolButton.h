#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QtCore/QVariant>

#include <QtWidgets/QWidget>
#include <QtGui/QPixmap>

class VariantAnimation;

class ToolButton : public QWidget
{
    Q_OBJECT

public:
    ToolButton(QWidget *parent = 0);
    ToolButton(const QPixmap &pixmap, QWidget *parent = 0);

    virtual
    QSize
    sizeHint() const;

    void
    addImage(const QPixmap &pixmap);

    int
    imageIndex() const { return pixIndex; }

    void
    setImageIndex(int index);

    QString
    text() const { return buttonText; }

    void
    setText(const QString &text);

    QVariant
    data() const { return itemData; }

    void
    clearAnimation();

signals:
    void
    pressed();

    void
    clicked();

public slots:
    void
    setData(const QVariant &value);

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

    virtual
    bool
    event(QEvent *event);

private slots:
    void
    animValueChanged(const QVariant &value);

private:
    void
    construct();

    QVector<QPixmap> pix;
    VariantAnimation *anim;

    int pixIndex;

    QString buttonText;

    bool down;
    bool inBounds;

    QVariant itemData;
};

#endif // TOOLBUTTON_H
