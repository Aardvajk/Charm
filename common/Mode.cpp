#include "Mode.h"

QString
Mode::toString(Type type)
{
    switch(type)
    {
        case Model  : return "Model";
        case Animate: return "Animate";

        default: break;
    }

    return QString();
}

