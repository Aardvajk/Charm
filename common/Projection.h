#ifndef PROJECTION_H
#define PROJECTION_H

#include <QtCore/QString>

namespace Projection
{

enum Type { Perspective, Orthographic };

QString
toString(Type type);

}

#endif // PROJECTION_H
