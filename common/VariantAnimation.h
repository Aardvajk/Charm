#ifndef VARIANTANIMATION_H
#define VARIANTANIMATION_H

#include <QtCore/QVariantAnimation>

class VariantAnimation : public QVariantAnimation
{
public:
    VariantAnimation(QObject *parent = 0) : QVariantAnimation(parent) { }

    virtual
    void
    updateCurrentValue(const QVariant &value){ }
};

#endif // VARIANTANIMATION_H
