#ifndef OBJFILECONVERTER_H
#define OBJFILECONVERTER_H

#include "Converter.h"

class ObjFileConverter : public Converter
{
public:
    ObjFileConverter();

    virtual
    bool
    toModel(const QString &path, Model *model);

    virtual
    bool
    fromModel(float scale, const QString &path, Model *model);
};

#endif // OBJFILECONVERTER_H
