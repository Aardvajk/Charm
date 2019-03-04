#include "Graphics.h"

#include "graphics/GraphicsDevice.h"
#include "graphics/BasicVertex.h"
#include "graphics/PointVertex.h"
#include "graphics/VertexBuffer.h"
#include "graphics/Texture.h"

Graphics::Graphics(QObject *parent)
    : QObject(parent),
      tempBuffer(0),
      cursorTex(0),
      registeredError(false)
{
    graphicsDevice = new GraphicsDevice(this);
}

bool
Graphics::initialise()
{
    return graphicsDevice->initialise();
}

void
Graphics::scheduleReset()
{
    graphicsDevice->scheduleReset();
}

bool
Graphics::reset()
{
    releaseDeviceBoundResources();
    emit aboutToReset();

    if(!graphicsDevice->reset())
    {
        return false;
    }

    if(!acquireDeviceBoundResources())
    {
        return false;
    }

    emit deviceReset();
    if(registeredError)
    {
        releaseDeviceBoundResources();
        return false;
    }

    return true;
}

void
Graphics::registerError()
{
    registeredError = true;
}

bool
Graphics::isLost() const
{
    return graphicsDevice->isLost();
}

bool
Graphics::needsResetting() const
{
    return graphicsDevice->needsResetting();
}

bool
Graphics::isReadyToReset() const
{
    return graphicsDevice->isReadyToReset();
}

bool
Graphics::acquireDeviceBoundResources()
{
    if(graphicsDevice->isLost())
    {
        return true;
    }

    declarations.append(new VertexDeclaration(graphicsDevice, this));
    if(!declarations.back()->acquire(BasicVertex::declaration()))
    {
        releaseDeviceBoundResources();
        return false;
    }

    declarations.append(new VertexDeclaration(graphicsDevice, this));
    if(!declarations.back()->acquire(PointVertex::declaration()))
    {
        releaseDeviceBoundResources();
        return false;
    }

    vertexShaders.append(new VertexShader(graphicsDevice, this));
    if(!vertexShaders.back()->acquire(":/resources/shaders/wireframevertex.txt"))
    {
        releaseDeviceBoundResources();
        return false;
    }

    vertexShaders.append(new VertexShader(graphicsDevice, this));
    if(!vertexShaders.back()->acquire(":/resources/shaders/screenvertex.txt"))
    {
        releaseDeviceBoundResources();
        return false;
    }

    vertexShaders.append(new VertexShader(graphicsDevice, this));
    if(!vertexShaders.back()->acquire(":/resources/shaders/basicvertex.txt"))
    {
        releaseDeviceBoundResources();
        return false;
    }

    tempBuffer = new VertexBuffer(graphicsDevice, this);
    if(!tempBuffer->acquire(1000 * sizeof(BasicVertex), D3DUSAGE_DYNAMIC, D3DPOOL_DEFAULT))
    {
        releaseDeviceBoundResources();
        return false;
    }

    cursorTex = new Texture(graphicsDevice, this);
    if(!cursorTex->acquire(":/resources/images/cursor.png"))
    {
        releaseDeviceBoundResources();
        return false;
    }

    return true;
}

void
Graphics::releaseDeviceBoundResources()
{
    qDeleteAll(declarations);
    declarations.clear();

    qDeleteAll(vertexShaders);
    vertexShaders.clear();

    delete tempBuffer;
    tempBuffer = 0;

    delete cursorTex;
    cursorTex = 0;
}

VertexDeclaration*
Graphics::setVertexDeclaration(int index)
{
    graphicsDevice->setVertexDeclaration(declarations[index]);
    return declarations[index];
}

VertexShader*
Graphics::setVertexShader(int index)
{
    graphicsDevice->setVertexShader(vertexShaders[index]);
    vertexShaders[index]->setDefaults();

    return vertexShaders[index];
}
