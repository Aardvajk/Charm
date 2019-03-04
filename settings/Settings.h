#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QVector>

class Settings : public QObject
{
public:
    Settings(QObject *parent = 0) : QObject(parent) { }
    Settings(const QString &name, const QVariant &value) : nodeName(name), nodeValue(value) { }
    ~Settings();

    void
    clear();

    QString
    name() const { return nodeName; }

    void
    setName(const QString &name);

    QVariant
    value(const QVariant &defaultValue = QVariant()) const { return nodeValue.isValid() ? nodeValue : defaultValue; }

    void
    setValue(const QVariant &value);

    Settings*
    add(const QString &name, const QVariant &value = QVariant());

    int
    childCount() const { return nodeChildren.count(); }

    Settings*
    child(const QString &name);

    Settings*
    child(int index);

    bool
    contains(const QString &name) const;

    static
    bool
    loadFromXml(const QString &path, Settings *settings);

    static
    bool
    saveToXml(const QString &path, Settings *settings);

private:
    QString nodeName;
    QVariant nodeValue;
    QVector<Settings*> nodeChildren;
};

#endif // SETTINGS_H
