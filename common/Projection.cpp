#include "Projection.h"

QString
Projection::toString(Projection::Type type)
{
    switch(type)
    {
        case Perspective : return "Perspective";
        case Orthographic: return "Orthographic";

        default: return QString();
    }
}
