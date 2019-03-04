#ifndef WIREFRAMEBUFFER_H
#define WIREFRAMEBUFFER_H

#include "Buffer.h"

class Model;

class WireFrameBuffer : public Buffer
{
    Q_OBJECT

public:
    WireFrameBuffer(Graphics *graphics, Model *model, QObject *parent = 0);

protected:
    virtual
    void
    generate(VertexBuffer *buffer);

private:
    Model *model;
};

#endif // WIREFRAMEBUFFER_H
