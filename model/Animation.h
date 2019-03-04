#ifndef ANIMATION_H
#define ANIMATION_H

#include <QtCore/QString>
#include <QtCore/QVector>

#include "common/StoredItem.h"

#include "model/KeyFrame.h"
#include "model/AnimationEvent.h"

class QDataStream;

class AnimationDetails
{
public:
    explicit AnimationDetails(const QString &name = QString()) : name(name), frames(30), duration(2.0f) { }

    QString name;
    int frames;
    float duration;
};

class Animation
{
public:
    Animation();
    explicit Animation(const AnimationDetails &details);

    bool
    isNull() const { return det.frames == 0; }

    QString
    name() const { return det.name; }

    int
    frames() const { return det.frames; }

    float
    duration() const { return det.duration; }

    AnimationDetails
    details() const { return det; }

    void
    setDetails(const AnimationDetails &details);

    KeyFrame
    keyFrame(int index) const;

    KeyFrame
    interpolatedKeyFrame(float position) const;

    void
    setKeyFrame(int index, const KeyFrame &key);

    void
    addKeyFrame(const KeyFrame &key);

    int
    keyFrameCount() const { return keys.count(); }

    void
    removeKeyFrame(int index);

    QVector<float>
    keyFramePositions() const;

    AnimationEvent
    event(int index) const;

    void
    setEvent(int index, const AnimationEvent &event);

    void
    addEvent(const AnimationEvent &event);

    int eventCount() const { return events.count(); }

    void
    removeEvent(int index);

    QVector<float>
    eventPositions() const;

    int
    indexForNewKeyFramePosition(float position) const;

    int
    indexForNewEventPosition(float position) const;

    void
    addJoints(int count);

    void
    removeJoints(const QVector<int> &indices);

    void
    write(QDataStream &stream, int version) const;

    void
    read(QDataStream &stream, int version);

private:
    AnimationDetails det;
    QVector<KeyFrame> keys;
    QVector<AnimationEvent> events;
};

typedef StoredItem<Animation> StoredAnimation;

#endif // ANIMATION_H
