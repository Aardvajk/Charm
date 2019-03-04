#include "Paint.h"

QString
Paint::toString(Paint::Type type)
{
    switch(type)
    {
        case None: return "None";
        case Palette: return "Palette";
        case Joints: return "Joints";

        default: return QString();
    }
}

