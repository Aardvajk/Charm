#ifndef PAINT_H
#define PAINT_H

#include <QtCore/QString>

namespace Paint
{


enum Type { None, Palette, Joints };

QString
toString(Type type);

}

#endif // PAINT_H
