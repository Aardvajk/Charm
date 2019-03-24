#ifndef LEVELCONVERTER_H
#define LEVELCONVERTER_H

#include "Converter.h"

class LevelConverter : public Converter
{
public:
    LevelConverter();

    virtual
    bool
    toModel(const QString &path, Model *model);

    virtual
    bool
    fromModel(float scale, const Vec3 &offset, const QString &path, Model *model);
};

#endif // LEVELCONVERTER_H
