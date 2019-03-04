#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H

#include <QtWidgets/QFrame>

class VariantAnimation;

class PopUpWindow : public QFrame
{
    Q_OBJECT

public:
    PopUpWindow(QWidget *widget, QWidget *parent = 0);

    void
    setSlideSizes(const QSize &start, const QSize &target);

    void
    setRelatedWidget(QWidget *widget);

    QWidget*
    widget() const { return child; }

signals:
    void
    hidden();

protected:
    virtual
    void
    showEvent(QShowEvent *event);

    virtual
    void
    hideEvent(QHideEvent *event);

private slots:
    void
    animationChanged(const QVariant &value);

private:
    bool
    isRelatedWidget(QObject *object) const;

    QWidget *child;
    VariantAnimation *animation;

    QSize startSize;
    QSize targetSize;

    QObject *relatedWidget;
};

#endif // POPUPWINDOW_H
