#ifndef AXIS_H
#define AXIS_H

#include <QtCore/QString>

#include "maths/Vec3.h"

namespace Axis
{

enum Type { X, Y, Z, None };

QString
toString(Type axis);

Vec3
vector(Type axis);

}

#endif // AXIS_H
