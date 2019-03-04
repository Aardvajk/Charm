#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QtCore/QObject>
#include <QtCore/QVector>

#include "graphics/VertexDeclaration.h"
#include "graphics/VertexShader.h"

class GraphicsDevice;
class VertexBuffer;
class Texture;

class Graphics : public QObject
{
    Q_OBJECT

public:
    Graphics(QObject *parent = 0);

    bool
    initialise();

    void
    scheduleReset();

    bool
    reset();

    void
    registerError();

    bool
    isLost() const;

    bool
    needsResetting() const;

    bool
    isReadyToReset() const;

    GraphicsDevice*
    device() const { return graphicsDevice; }

    VertexDeclaration*
    setBasicVertexDeclaration(){ return setVertexDeclaration(0); }

    VertexDeclaration*
    setPointVertexDeclaration(){ return setVertexDeclaration(1); }

    VertexShader*
    setWireFrameVertexShader(){ return setVertexShader(0); }

    VertexShader*
    setScreenVertexShader(){ return setVertexShader(1); }

    VertexShader*
    setBasicVertexShader(){ return setVertexShader(2); }

    VertexBuffer*
    temporaryVertexBuffer() const { return tempBuffer; }

    Texture*
    cursorTexture() const { return cursorTex; }

signals:
    void
    aboutToReset();

    void
    deviceReset();

private:
    bool
    acquireDeviceBoundResources();

    void
    releaseDeviceBoundResources();

    VertexDeclaration*
    setVertexDeclaration(int index);

    VertexShader*
    setVertexShader(int index);

    GraphicsDevice *graphicsDevice;

    QVector<VertexDeclaration*> declarations;
    QVector<VertexShader*> vertexShaders;

    VertexBuffer *tempBuffer;

    Texture *cursorTex;

    bool registeredError;
};

#endif // GRAPHICS_H
