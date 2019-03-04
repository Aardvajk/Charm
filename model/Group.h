#ifndef GROUP_H
#define GROUP_H

#include <QtCore/QString>

#include "common/StoredItem.h"

class GroupDetails
{
public:
    GroupDetails(const QString &name = "New Group") : name(name) { }

    QString name;
};

class Group
{
public:
    Group(int id = 0) : id(id) { }

    bool
    isNull() const { return !id; }

    int id;
    GroupDetails details;
};

typedef StoredItem<Group> StoredGroup;

#endif // GROUP_H
