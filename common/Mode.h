#ifndef MODE_H
#define MODE_H

#include <QtCore/QString>

namespace Mode
{

enum Type { Model, Animate, Null };

QString
toString(Type type);

}

#endif // MODE_H
