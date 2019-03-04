#include "Buffer.h"

#include "debug/Assert.h"

#include "graphics/Graphics.h"
#include "graphics/VertexBuffer.h"

Buffer::Buffer(Graphics *graphics, int bytes, QObject *parent)
    : QObject(parent),
      graphics(graphics),
      bytes(bytes),
      data(0),
      valid(false)
{
    if(!graphics->isLost())
    {
        acquire();
    }

    connect(graphics, SIGNAL(aboutToReset()), SLOT(release()));
    connect(graphics, SIGNAL(deviceReset()), SLOT(acquire()));
}

VertexBuffer*
Buffer::buffer()
{
    if(!valid)
    {
        generate(data);
        valid = true;
    }

    return data;
}

void
Buffer::invalidate()
{
    valid = false;
}

void
Buffer::acquire()
{
    data = new VertexBuffer(graphics->device(), this);

    if(!data->acquire(bytes, D3DUSAGE_DYNAMIC, D3DPOOL_DEFAULT))
    {
        release();
        graphics->registerError();
    }
}

void
Buffer::release()
{
    delete data;
    data = 0;
    valid = false;
}

