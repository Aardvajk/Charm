#include "ModelBuffers.h"

#include "graphics/Graphics.h"

#include "model/Model.h"

#include "buffers/PreviewBuffer.h"
#include "buffers/WireFrameBuffer.h"
#include "buffers/PointBuffer.h"
#include "buffers/JointBuffer.h"

ModelBuffers::ModelBuffers(Graphics *graphics, Model *model, float jointRadius, QObject *parent)
    : QObject(parent),
      graphics(graphics),
      jointRad(jointRadius)
{
    preview = new PreviewBuffer(graphics, model, this);
    wireFrame = new WireFrameBuffer(graphics, model, this);
    point = new PointBuffer(graphics, model, this);

    normalJoint = new JointBuffer(graphics, jointRadius, D3DCOLOR_XRGB(0, 255, 0), this);
    selectedJoint = new JointBuffer(graphics, jointRadius, D3DCOLOR_XRGB(255, 0, 0), this);
}
