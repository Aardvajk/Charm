#include "Property.h"

Property::Property(QObject *parent)
    : QObject(parent),
      propertyType(Null)
{
}

Property::Property(Type type, const QString &name, const QVariant &value, QObject *parent)
    : QObject(parent),
      propertyType(type),
      propertyName(name),
      propertyValue(value)
{
}

void
Property::setRange(const QVariant &min, const QVariant &max)
{
    propertyMin = min;
    propertyMax = max;
}

void
Property::setValue(const QVariant &value)
{
    propertyValue = value;
}
