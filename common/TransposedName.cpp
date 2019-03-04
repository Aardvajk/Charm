#include "TransposedName.h"

#include <QtCore/QStringList>

QString
transposedName(const QString &name)
{
    QStringList tokens = name.split(' ');
    for(int i = 0; i < tokens.count(); ++i)
    {
        QString token = tokens[i];

        if(token == "Left") tokens[i] = "Right";
        if(token == "Right") tokens[i] = "Left";

        if(token == "left") tokens[i] = "right";
        if(token == "right") tokens[i] = "left";
    }

    return tokens.join(" ");
}

