#include "VariantSet.h"

VariantSet::VariantSet()
{
}

VariantSet::VariantSet(const QList<QVariant> &values)
{
    foreach(const QVariant &value, values)
    {
        data.append(value);
    }
}

void
VariantSet::clear()
{
    data.clear();
}

void
VariantSet::insert(const QVariant &value)
{
    if(!data.contains(value))
    {
        data.append(value);
    }
}

bool
VariantSet::contains(const QVariant &value) const
{
    return data.contains(value);
}

QList<QVariant>
VariantSet::toList() const
{
    QList<QVariant> result;
    foreach(const QVariant &value, data)
    {
        result.append(value);
    }

    return result;
}
