#ifndef ACTION_H
#define ACTION_H

#include <QtCore/QList>
#include <QtWidgets/QAction>

class Action : public QAction
{
    Q_OBJECT

public:
    Action(QObject *parent = 0);
    Action(const QString &text, QObject *parent = 0);
    Action(const QIcon &icon, const QString &text, QObject *parent = 0);

    QString group() const { return groupText; }
    void setGroup(const QString &group);

private:
    QString groupText;
};

#endif // ACTION_H
