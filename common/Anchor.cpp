#include "Anchor.h"

QString
Anchor::toString(Type anchor)
{
    switch(anchor)
    {
        case Selection: return "Selection";
        case Cursor   : return "Cursor";
        case Origin   : return "Origin";

        default: break;
    }

    return QString();
}

QString
Anchor::toCode(Type anchor)
{
    switch(anchor)
    {
        case Selection: return "S";
        case Cursor   : return "C";
        case Origin   : return "O";

        default: break;
    }

    return QString();
}

