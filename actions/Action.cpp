#include "Action.h"

Action::Action(QObject *parent) : QAction(parent)
{
}

Action::Action(const QString &text, QObject *parent) : QAction(text, parent)
{
}

Action::Action(const QIcon &icon, const QString &text, QObject *parent) : QAction(icon, text, parent)
{
}

void Action::setGroup(const QString &group)
{
    if(groupText != group)
    {
        groupText = group;
        emit changed();
    }
}
