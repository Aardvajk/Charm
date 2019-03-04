#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <QtCore/QObject>

#include <d3d9.h>

class GraphicsDevice;

class VertexBuffer : public QObject
{
    Q_OBJECT

public:
    VertexBuffer(GraphicsDevice *graphics, QObject *parent = 0);
    ~VertexBuffer();

    bool
    acquire(size_t bytes, DWORD usage, D3DPOOL pool);

    void
    release();

    void
    begin(DWORD flags);

    void
    end();

    template<class T>
    void
    add(const T &t){ addData(reinterpret_cast<const void*>(&t), sizeof(T), 1); }

    int
    count() const { return no; }

    int
    capacity() const { return max; }

    IDirect3DVertexBuffer9*
    getVertexBuffer() const { return ptr; }

private:
    void*
    lock(DWORD flags);

    void
    unlock();

    bool
    addData(const void *data, size_t str, size_t count);

    GraphicsDevice *graphics;
    IDirect3DVertexBuffer9 *ptr;

    size_t max;
    size_t no;
    BYTE *current;
};

#endif // VERTEXBUFFER_H
