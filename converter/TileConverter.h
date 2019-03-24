#ifndef TILECONVERTER_H
#define TILECONVERTER_H

#include "Converter.h"

class TileConverter : public Converter
{
public:
    TileConverter();

    virtual
    bool
    toModel(const QString &path, Model *model);

    virtual
    bool
    fromModel(float scale, const Vec3 &offset, const QString &path, Model *model);
};

#endif // TILECONVERTER_H
