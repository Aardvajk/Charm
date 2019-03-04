#include "VertexDeclaration.h"
#include "GraphicsDevice.h"

VertexElement::VertexElement()
{
    val.Stream = 0xFF;
    val.Offset = 0;
    val.Type = D3DDECLTYPE_UNUSED;
    val.Method = 0;
    val.Usage = 0;
    val.UsageIndex = 0;
}

VertexElement::VertexElement(WORD stream, WORD offset, BYTE type, BYTE usage, BYTE index)
{
    val.Stream = stream;
    val.Offset = offset;
    val.Type = type;
    val.Method = D3DDECLMETHOD_DEFAULT;
    val.Usage = usage;
    val.UsageIndex = index;
}

VertexDeclaration::VertexDeclaration(GraphicsDevice *graphics, QObject *parent)
    : QObject(parent),
      graphics(graphics),
      ptr(0)
{
}

VertexDeclaration::~VertexDeclaration()
{
    release();
}

bool
VertexDeclaration::acquire(const QVector<VertexElement> &elements)
{
    release();

    HRESULT R = graphics->getDevice()->CreateVertexDeclaration(reinterpret_cast<const D3DVERTEXELEMENT9*>(&(elements[0])), &ptr);

    if(FAILED(R)) return false;

    return true;
}

void
VertexDeclaration::release()
{
    if(ptr)
    {
        ptr->Release();
        ptr = 0;
    }
}
