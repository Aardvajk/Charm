#ifndef XFILECONVERTER_H
#define XFILECONVERTER_H

#include "Converter.h"

class Graphics;
class ID3DXMesh;
class ID3DXBuffer;

class XFileConverter : public Converter
{
public:
    XFileConverter(Graphics *graphics);

    virtual
    bool
    toModel(const QString &path, Model *model);

    virtual
    bool
    fromModel(float scale, const QString &path, Model *model);

private:
    bool
    release();

    Graphics *graphics;

    ID3DXMesh *mesh;
    ID3DXBuffer *adjBuffer;
    ID3DXBuffer *materialBuffer;
};

#endif // XFILECONVERTER_H
