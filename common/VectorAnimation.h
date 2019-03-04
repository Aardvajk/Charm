#ifndef VECTORANIMATION_H
#define VECTORANIMATION_H

#include <QtCore/QObject>

#include "maths/Vec3.h"

class VariantAnimation;

class VectorAnimation : public QObject
{
    Q_OBJECT

public:
    VectorAnimation(QObject *parent = 0);

    void
    setStartValue(const Vec3 &v);

    void
    setEndValue(const Vec3 &v);

    void
    setDuration(int msecs);

    void
    start();

    void
    stop();

    bool
    playing() const;

signals:
    void
    vectorChanged(const Vec3 &v);

private slots:
    void
    valueUpdated(const QVariant &value);

private:
    VariantAnimation *anim;

    Vec3 begin;
    Vec3 end;
};

#endif // VECTORANIMATION_H
