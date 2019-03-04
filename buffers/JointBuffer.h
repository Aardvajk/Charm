#ifndef JOINTBUFFER_H
#define JOINTBUFFER_H

#include "Buffer.h"

#include <d3d9.h>

class JointBuffer : public Buffer
{
    Q_OBJECT

public:
    JointBuffer(Graphics *graphics, float radius, D3DCOLOR color, QObject *parent = 0);

protected:
    virtual
    void
    generate(VertexBuffer *buffer);

private:
    float radius;
    D3DCOLOR color;
};

#endif // JOINTBUFFER_H
