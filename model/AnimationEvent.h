#ifndef ANIMATIONEVENT_H
#define ANIMATIONEVENT_H

#include "common/StoredItem.h"

#include <QtCore/QDebug>

class AnimationEventDetails
{
public:
    explicit AnimationEventDetails(float position, const QString &data = QString()) : position(position), data(data) { }

    float position;
    QString data;
};

class AnimationEvent
{
public:
    AnimationEvent() : details(-1) { }
    explicit AnimationEvent(float position) : details(position) { }

    bool
    isNull() const { return details.position < 0; }

    bool
    operator<(const AnimationEvent &event) const { return details.position < event.details.position; }

    AnimationEventDetails details;
};

typedef StoredItem<AnimationEvent> StoredAnimationEvent;

#endif // ANIMATIONEVENT_H
