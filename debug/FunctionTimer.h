#ifndef FUNCTIONTIMER_H
#define FUNCTIONTIMER_H

#include <QtCore/QTime>
#include <QtCore/QString>
#include <QtCore/QDebug>

#include "debug/DebugSettings.h"

#ifdef FUNCTIONTIMERS
class FuncTimer
{
public:
    FuncTimer(const QString &name) : name(name) { timer.start(); }
    ~FuncTimer(){ qDebug() << name << "took" << timer.elapsed() << "ms"; }

    QTime timer;
    QString name;
};

#define TimeFunction FuncTimer __funcTimer(__PRETTY_FUNCTION__)
#else
#define TimeFunction void(0)
#endif

#endif // FUNCTIONTIMER_H
