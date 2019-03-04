#include "VectorAnimation.h"

#include "maths/Maths.h"

#include "common/VariantAnimation.h"

VectorAnimation::VectorAnimation(QObject *parent)
    : QObject(parent)
{
    anim = new VariantAnimation(this);
    connect(anim, SIGNAL(valueChanged(QVariant)), SLOT(valueUpdated(QVariant)));
}

void
VectorAnimation::setStartValue(const Vec3 &v)
{
    begin = v;
}

void
VectorAnimation::setEndValue(const Vec3 &v)
{
    end = v;
}

void
VectorAnimation::setDuration(int msecs)
{
    anim->setDuration(msecs);
}

void
VectorAnimation::start()
{
    anim->stop();

    anim->setStartValue(1.0f);
    anim->setEndValue(0.0f);

    anim->start();
}

void
VectorAnimation::stop()
{
    anim->stop();
}

bool
VectorAnimation::playing() const
{
    return anim->state() == QAbstractAnimation::Running;
}

void
VectorAnimation::valueUpdated(const QVariant &value)
{
    emit vectorChanged(interpolateVectors(begin, end, value.toFloat()));
}
