#ifndef BUFFER_H
#define BUFFER_H

#include <QtCore/QObject>

class Graphics;
class VertexBuffer;

class Buffer : public QObject
{
    Q_OBJECT

public:
    Buffer(Graphics *graphics, int bytes, QObject *parent = 0);

    VertexBuffer*
    buffer();

public slots:
    void
    invalidate();

protected:
    virtual
    void
    generate(VertexBuffer *buffer) = 0;

private slots:
    void
    acquire();

    void
    release();

private:
    Graphics *graphics;
    int bytes;
    VertexBuffer *data;

    mutable bool valid;
};

#endif // BUFFER_H
