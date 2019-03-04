#ifndef STOREDITEM_H
#define STOREDITEM_H

#include <QtGui/QColor>

template<class T> class StoredItem
{
public:
    StoredItem() : index(-1) { }
    StoredItem(int index, const T &old, const T &value) : index(index), old(old), value(value) { }

    int index;

    T old;
    T value;
};

typedef StoredItem<int> StoredInt;
typedef StoredItem<QColor> StoredColor;

#endif // STOREDITEM_H
