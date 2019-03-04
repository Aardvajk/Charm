#ifndef RAWBUFFERCONVERTER_H
#define RAWBUFFERCONVERTER_H

#include "Converter.h"

class RawBufferConverter : public Converter
{
public:
    RawBufferConverter(bool boneNames, bool faceColors);

    virtual
    bool
    toModel(const QString &path, Model *model);

    virtual
    bool
    fromModel(float scale, const QString &path, Model *model);

private:
    bool boneNames;
    bool faceColors;
};

#endif // RAWBUFFERCONVERTER_H
