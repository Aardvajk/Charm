#ifndef STATICBUFFERCONVERTER_H
#define STATICBUFFERCONVERTER_H

#include "Converter.h"

class StaticBufferConverter : public Converter
{
public:
    StaticBufferConverter(bool indexed, bool texCoords, bool dummyTangent);

    virtual
    bool
    toModel(const QString &path, Model *model);

    virtual
    bool
    fromModel(float scale, const QString &path, Model *model);

private:
    bool indexed;
    bool texCoords;
    bool dummyTangent;
};

#endif // STATICBUFFERCONVERTER_H
