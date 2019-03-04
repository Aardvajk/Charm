#include "ModelElement.h"

QString
ModelElement::toString(ModelElement::Type type)
{
    switch(type)
    {
        case Vertex: return "Vertices";
        case Face  : return "Faces";
        case Joint : return "Joints";

        default: return QString();
    }
}

