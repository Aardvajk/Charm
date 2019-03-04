#ifndef ROTATION_H
#define ROTATION_H

#include <QtCore/QString>

#include "maths/Vec3.h"

namespace Rotation
{

enum Type { Front, Back, Left, Right, Top, Bottom, User };

Vec3
toVector(Type type);

QString
toString(Type type);

}

#endif // ROTATION_H
