#ifndef MODELBUFFERS_H
#define MODELBUFFERS_H

#include <QtCore/QObject>

class Graphics;
class Model;
class Buffer;
class PreviewBuffer;

class ModelBuffers : public QObject
{
    Q_OBJECT

public:
    ModelBuffers(Graphics *graphics, Model *model, float jointRadius, QObject *parent = 0);

    PreviewBuffer*
    previewBuffer() const { return preview; }

    Buffer*
    wireFrameBuffer() const { return wireFrame; }

    Buffer*
    pointBuffer() const { return point; }

    Buffer*
    normalJointBuffer() const { return normalJoint; }

    Buffer*
    selectedJointBuffer() const { return selectedJoint; }

    float
    jointRadius() const { return jointRad; }

private:
    Graphics *graphics;

    PreviewBuffer *preview;
    Buffer *wireFrame;
    Buffer *point;

    Buffer *normalJoint;
    Buffer *selectedJoint;

    float jointRad;
};

#endif // MODELBUFFERS_H
