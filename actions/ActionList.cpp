#include "ActionList.h"

#include "actions/Action.h"

#include "settings/Settings.h"

#include <QtWidgets/QWidget>

namespace
{

bool actionLess(Action *a, Action *b)
{
    return QString::compare(a->objectName(), b->objectName()) < 0;
}

}

ActionList::ActionList(QWidget *parent) : QObject(parent), ownerWidget(parent)
{
}

Action *ActionList::add(const QString &group, const QString &name)
{
    Action *action = new Action(this);

    action->setObjectName(name);
    action->setGroup(group);

    ownerWidget->addAction(action);

    data.append(action);

    return action;
}

Action *ActionList::find(const QString &group, const QString &name) const
{
    for(int i = 0; i < data.count(); ++i)
    {
        if(data[i]->group() == group && data[i]->objectName() == name)
        {
            return data[i];
        }
    }

    return 0;
}

QMap<QString, QList<Action*> > ActionList::toMap() const
{
    QMap<QString, QList<Action*> > map;
    for(int i = 0; i < data.count(); ++i)
    {
        map[data[i]->group()].append(data[i]);
    }

    foreach(const QString &key, map.keys())
    {
        qSort(map[key].begin(), map[key].end(), actionLess);
    }

    return map;
}

void ActionList::loadFromSettings(Settings *settings)
{
    for(int i = 0; i < settings->childCount(); ++i)
    {
        Settings *value = settings->child(i);

        Action *action = find(value->child("Group")->value().toString(), value->child("Name")->value().toString());
        if(action)
        {
            action->setShortcut(QKeySequence(value->child("Shortcut")->value().toString()));
        }
    }
}

void ActionList::saveToSettings(Settings *settings) const
{
    settings->clear();
    for(int i = 0; i < data.count(); ++i)
    {
        Settings *child = settings->add("Action");

        child->add("Group", data[i]->group());
        child->add("Name", data[i]->objectName());
        child->add("Shortcut", data[i]->shortcut().toString(QKeySequence::NativeText));
    }
}
