#ifndef SHADE_H
#define SHADE_H

#include <QtCore/QString>

namespace Shade
{

enum Type { None, Smooth, Flat };

QString
toString(Type type);

}

#endif // SHADE_H
