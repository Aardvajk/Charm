#ifndef ASSERT_H
#define ASSERT_H

#include <QtCore/QString>

#include "debug/DebugSettings.h"

class AssertException { };

#ifdef ASSERTIONS
void
AssertImplementation(bool value, const QString &expr, const QString &file, const QString &method, int line);

#define Assert(x) AssertImplementation(x, #x, __FILE__, __PRETTY_FUNCTION__, __LINE__)
#else
#define Assert(x) void(0)
#endif

#endif // ASSERT_H
