#ifndef MODELELEMENT_H
#define MODELELEMENT_H

#include <QtCore/QString>

namespace ModelElement
{

enum Type { Vertex, Face, Joint };

QString
toString(Type type);

}

#endif // MODELELEMENT_H
