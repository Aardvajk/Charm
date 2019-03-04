#include "VertexBuffer.h"
#include "GraphicsDevice.h"

#include "debug/Assert.h"

VertexBuffer::VertexBuffer(GraphicsDevice *graphics, QObject *parent)
    : QObject(parent),
      graphics(graphics),
      ptr(0),
      max(0),
      no(0),
      current(0)
{
}

VertexBuffer::~VertexBuffer()
{
    release();
}

bool
VertexBuffer::acquire(size_t bytes, DWORD usage, D3DPOOL pool)
{
    release();

    HRESULT r = graphics->getDevice()->CreateVertexBuffer(bytes, usage, 0, pool, &ptr, 0);

    if(FAILED(r))
    {
        ptr = 0;
        return false;
    }

    max = bytes;
    no = 0;
    current = 0;

    return true;
}

void
VertexBuffer::release()
{
    if(ptr)
    {
        ptr->Release();
        ptr = 0;

        max = 0;
        no = 0;
        current = 0;
    }
}

void
VertexBuffer::begin(DWORD flags)
{
    end();

    current = reinterpret_cast<BYTE*>(lock(flags));
    no = 0;
}

void
VertexBuffer::end()
{
    if(current)
    {
        unlock();
        current = 0;
    }
}

void*
VertexBuffer::lock(DWORD flags)
{
    if(!ptr) return 0;

    void *data;
    HRESULT r = ptr->Lock(0, 0, &data, flags);

    if(FAILED(r)) return 0;

    return data;
}

void
VertexBuffer::unlock()
{
    if(ptr) ptr->Unlock();
}

bool
VertexBuffer::addData(const void *data, size_t str, size_t count)
{
    Assert(current);
    Assert(no + count < max);

    const BYTE *src = reinterpret_cast<const BYTE*>(data);

    std::copy(src, src + (count * str), current);

    current += (count * str);
    no += count * str;

    return true;
}

