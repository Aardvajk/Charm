#ifndef ACTIONLIST_H
#define ACTIONLIST_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QMap>

class QWidget;
class Action;
class Settings;

class ActionList : public QObject
{
    Q_OBJECT

public:
    ActionList(QWidget *parent = 0);

    Action *add(const QString &group, const QString &name);
    Action *find(const QString &group, const QString &name) const;

    QMap<QString, QList<Action*> > toMap() const;

    void loadFromSettings(Settings *settings);
    void saveToSettings(Settings *settings) const;

private:
    QWidget *ownerWidget;
    QList<Action*> data;
};

#endif // ACTIONLIST_H
