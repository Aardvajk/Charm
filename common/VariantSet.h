#ifndef VARIANTSET_H
#define VARIANTSET_H

#include <QtCore/QVariant>
#include <QtCore/QVector>
#include <QtCore/QList>

class VariantSet
{
public:
    VariantSet();
    VariantSet(const QList<QVariant> &values);

    bool
    isEmpty() const { return data.isEmpty(); }

    void
    clear();

    void
    insert(const QVariant &value);

    VariantSet&
    operator<<(const QVariant &value){ insert(value); return *this; }

    bool
    contains(const QVariant &value) const;

    QList<QVariant>
    toList() const;

private:
    QVector<QVariant> data;
};

#endif // VARIANTSET_H
