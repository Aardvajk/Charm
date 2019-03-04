#ifndef FATAL_H
#define FATAL_H

#include <QtCore/QString>

class FatalException { };

void
Fatal(const QString &error);

#endif // FATAL_H
