#ifndef PROPERTY_H
#define PROPERTY_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QString>

class Property : public QObject
{
    Q_OBJECT

public:
    enum Type { Int, Float, Bool, Null };

    Property(QObject *parent = 0);
    Property(Type type, const QString &name, const QVariant &value, QObject *parent =  0);

    Type
    type() const { return propertyType; }

    QString
    name() const { return propertyName; }

    QVariant
    value() const { return propertyValue; }

    QVariant
    min() const { return propertyMin; }

    QVariant
    max() const { return propertyMax; }

    void
    setRange(const QVariant &min, const QVariant &max);

    void
    setValue(const QVariant &value);

private:
    Type propertyType;
    QString propertyName;
    QVariant propertyValue;

    QVariant propertyMin;
    QVariant propertyMax;
};

#endif // PROPERTY_H
