#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <QtCore/QVector>

#include "common/StoredItem.h"

#include "model/Joint.h"

class Model;
class ModelData;
class QDataStream;

class KeyFrame
{
public:
    explicit KeyFrame(float position = -1.0f);
    KeyFrame(float position, Model *model);
    explicit KeyFrame(Model *model);

    bool
    isNull() const { return pos < 0.0f && transforms.isEmpty(); }

    bool
    operator<(const KeyFrame &frame) const { return pos < frame.pos; }

    int
    count() const { return transforms.count(); }

    JointTransform
    transform(int index) const;

    float
    position() const { return pos; }

    void
    toModelData(ModelData *data) const;

    void
    addJoints(int count);

    void
    removeJoints(const QVector<int> &indices);

    void
    write(QDataStream &stream, int version) const;

    void
    read(QDataStream &stream, int version);

    KeyFrame
    interpolate(const KeyFrame &b, float t) const;

    void
    swap(int from, int to);

private:
    float pos;
    QVector<JointTransform> transforms;
};

typedef StoredItem<KeyFrame> StoredKeyFrame;

#endif // KEYFRAME_H
