#ifndef POINTBUFFER_H
#define POINTBUFFER_H

#include "Buffer.h"

class Model;

class PointBuffer : public Buffer
{
    Q_OBJECT

public:
    PointBuffer(Graphics *graphics, Model *model, QObject *parent = 0);

protected:
    virtual
    void
    generate(VertexBuffer *buffer);

private:
    Model *model;
};

#endif // POINTBUFFER_H
