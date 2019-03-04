#ifndef GRAPHICSDEVICE_H
#define GRAPHICSDEVICE_H

#include <QtCore/QObject>
#include <QtCore/QMap>

#include <d3d9.h>

class GraphicsWidget;
class SwapChain;
class VertexDeclaration;
class VertexShader;
class VertexBuffer;
class Texture;

class GraphicsDevice : public QObject
{
    Q_OBJECT

public:
    GraphicsDevice(QObject *parent = 0);
    ~GraphicsDevice();

    bool
    initialise();

    void
    scheduleReset();

    void
    registerWidget(GraphicsWidget *widget);

    void
    unregisterWidget(GraphicsWidget *widget);

    void
    begin(GraphicsWidget *widget, D3DCOLOR clearColor);

    void
    end(GraphicsWidget *widget);

    void
    setVertexDeclaration(const VertexDeclaration *dec = 0);

    void
    setVertexShader(const VertexShader *shader = 0);

    void
    setTexture(int stage, const Texture *texture = 0);

    void
    setCullMode(DWORD mode);

    void
    enableBias(bool state);

    void
    enableDepth(bool state);

    void
    enableDepthWrite(bool state);

    void
    enableBufferWrite(bool state);

    void
    enableAlphaBlend(bool state);

    void
    enablePointSpriteTexturing(bool state);

    void
    setPointSize(float size);

    void
    renderTriangleList(const VertexBuffer *buffer, size_t stride);

    void
    renderTriangleListGroup(const VertexBuffer *buffer, size_t stride, int start, int count);

    void
    renderLineList(const VertexBuffer *buffer, size_t stride);

    void
    renderPointList(const VertexBuffer *buffer, size_t stride);

    bool
    isLost() const;

    bool
    needsResetting() const;

    bool
    isReadyToReset() const;

    bool
    reset();

    IDirect3DDevice9*
    getDevice() const;

protected:
    virtual
    bool
    eventFilter(QObject *object, QEvent *event);

private:
    void
    releaseDevice();

    IDirect3D9 *direct3d;
    IDirect3DDevice9 *device;

    IDirect3DSurface9 *backBuffer;

    QMap<GraphicsWidget*, SwapChain*> widgets;

    bool requiresReset;
};

#endif // GRAPHICSDEVICE_H
