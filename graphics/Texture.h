#ifndef TEXTURE_H
#define TEXTURE_H

#include <QtCore/QObject>
#include <QtCore/QSize>

#include <d3d9.h>

class GraphicsDevice;

class Texture : public QObject
{
    Q_OBJECT

public:
    Texture(GraphicsDevice *graphics, QObject *parent = 0);
    ~Texture();

    bool
    acquire(const QString &path);

    void
    release();

    QSize
    size() const { return nativeSize; }

    IDirect3DTexture9*
    getTexture() const { return ptr; }

private:
    GraphicsDevice *graphics;
    IDirect3DTexture9 *ptr;

    QSize nativeSize;
};

#endif // TEXTURE_H
