#include "Shade.h"

QString
Shade::toString(Shade::Type type)
{
    switch(type)
    {
        case None: return "None";
        case Smooth: return "Smooth";
        case Flat: return "Flat";

        default: return QString();
    }
}

