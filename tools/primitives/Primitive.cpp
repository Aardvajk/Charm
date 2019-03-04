#include "Primitive.h"

Primitive::Primitive(Model *model, QObject *parent)
    : QObject(parent),
      currentModel(model)
{
}

QVector<Property*>
Primitive::properties() const
{
    return QVector<Property*>();
}

void
Primitive::mousePressed(ModelView *view, QMouseEvent *event)
{
}

void
Primitive::mouseMoved(ModelView *view, QMouseEvent *event)
{
}

void
Primitive::mouseReleased(ModelView *view, QMouseEvent *event)
{
}

void
Primitive::focusLost()
{
}

void
Primitive::saveSettings(Settings *settings) const
{
}
