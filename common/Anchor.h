#ifndef ANCHOR_H
#define ANCHOR_H

#include <QtCore/QString>

namespace Anchor
{

enum Type { Selection, Cursor, Origin, Null };

QString
toString(Type anchor);

QString
toCode(Type anchor);

}

#endif // ANCHOR_H
