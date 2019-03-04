#include "PopUpWindow.h"

#include "common/VariantAnimation.h"

#include "maths/Maths.h"

PopUpWindow::PopUpWindow(QWidget *widget, QWidget *parent)
    : QFrame(parent, Qt::Popup),
      child(widget),
      relatedWidget(0)
{
    if(child)
    {
        child->setParent(this);
        child->move(0, 0);
    }

    setVisible(false);

    animation = new VariantAnimation(this);

    connect(animation, SIGNAL(valueChanged(QVariant)), SLOT(animationChanged(QVariant)));
}

void
PopUpWindow::setSlideSizes(const QSize &start, const QSize &target)
{
    startSize = start;
    targetSize = target;
}

void
PopUpWindow::setRelatedWidget(QWidget *widget)
{
    relatedWidget = widget;
}

void
PopUpWindow::showEvent(QShowEvent *event)
{
    resize(startSize);

    if(child)
    {
        child->resize(targetSize);
    }

    animation->setStartValue(0.0f);
    animation->setEndValue(1.0f);
    animation->setDuration(200);

    animation->start();
}

void
PopUpWindow::hideEvent(QHideEvent *event)
{
    emit hidden();
}

void
PopUpWindow::animationChanged(const QVariant &value)
{
    resize(interpolate(startSize, targetSize, value.toFloat()));

    if(child)
    {
        child->move(-(targetSize.width() - width()), -(targetSize.height() - height()));
    }
}

bool
PopUpWindow::isRelatedWidget(QObject *object) const
{
    if(object == this || findChildren<QObject*>().contains(object))
    {
        return true;
    }

    if(object == relatedWidget)
    {
        return true;
    }

    return false;
}
