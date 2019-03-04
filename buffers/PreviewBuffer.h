#ifndef PREVIEWBUFFER_H
#define PREVIEWBUFFER_H

#include "Buffer.h"

#include <QtCore/QVector>

class Model;

class PreviewBuffer : public Buffer
{
    Q_OBJECT

public:
    PreviewBuffer(Graphics *graphics, Model *model, QObject *parent = 0);

    QVector<int>
    groupCounts() const { return counts; }

protected:
    virtual
    void
    generate(VertexBuffer *buffer);

private:
    Model *model;
    QVector<int> counts;
};

#endif // PREVIEWBUFFER_H
