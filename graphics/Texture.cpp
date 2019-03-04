#include "Texture.h"
#include "GraphicsDevice.h"

#include <QtCore/QFile>
#include <QtCore/QByteArray>

#include <d3dx9.h>

Texture::Texture(GraphicsDevice *graphics, QObject *parent)
    : QObject(parent),
      graphics(graphics),
      ptr(0)
{
}

Texture::~Texture()
{
    release();
}

bool
Texture::acquire(const QString &path)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QByteArray src = file.readAll();

    release();

    HRESULT r = D3DXCreateTextureFromFileInMemoryEx(graphics->getDevice(), src.data(), src.length(), 0, 0, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0, 0, 0, &ptr);

    if(FAILED(r))
    {
        return false;
    }

    D3DSURFACE_DESC desc;
    ptr->GetLevelDesc(0, &desc);

    nativeSize.setWidth(desc.Width);
    nativeSize.setHeight(desc.Height);

    return true;
}

void
Texture::release()
{
    if(ptr)
    {
        ptr->Release();
        ptr = 0;
    }
}

