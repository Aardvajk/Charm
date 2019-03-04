#include "Animation.h"

#include "debug/Assert.h"

#include <QtCore/QDataStream>

Animation::Animation()
{
    det.frames = 0;
}

Animation::Animation(const AnimationDetails &details)
    : det(details)
{
}

void
Animation::setDetails(const AnimationDetails &details)
{
    det = details;

    for(int i = keys.count() - 1; i >= 0; --i)
    {
        if(keys[i].position() > det.frames)
        {
            keys.remove(i);
        }
    }
}

KeyFrame
Animation::keyFrame(int index) const
{
    Assert(index >= 0 && index < keys.count());
    return keys[index];
}

KeyFrame
Animation::interpolatedKeyFrame(float position) const
{
    float frames = det.frames;

    if(position < keys.front().position())
    {
        int a = keys.count() - 1;
        int b = 0;

        float range = (keys[b].position() + (frames - keys[a].position())) - 1;
        float offset = (position + (frames - keys[a].position())) - 1;

        return keys[a].interpolate(keys[b], offset / range);
    }

    if(position > keys.back().position())
    {
        int a = keys.count() - 1;
        int b = 0;

        float range = (keys[b].position() + (frames - keys[a].position())) - 1;
        float offset = position - keys[a].position();

        return keys[a].interpolate(keys[b], offset / range);
    }

    for(int i = 0; i < keys.count() - 1; ++i)
    {
        if(position >= keys[i].position() && position < keys[i + 1].position())
        {
            float range = keys[i + 1].position() - keys[i].position();
            float offset = position - keys[i].position();

            return keys[i].interpolate(keys[i + 1], offset / range);
        }
    }

    return keys[keys.count() - 1];
}

void
Animation::setKeyFrame(int index, const KeyFrame &key)
{
    Assert(index >= 0 && index < keys.count());
    keys[index] = key;
}

void
Animation::addKeyFrame(const KeyFrame &key)
{
    keys.append(key);
    qSort(keys);
}

void
Animation::removeKeyFrame(int index)
{
    Assert(index >= 0 && index < keys.count());
    keys.remove(index);
}

QVector<float>
Animation::keyFramePositions() const
{
    QVector<float> positions;
    foreach(const KeyFrame &key, keys)
    {
        positions.append(key.position());
    }

    return positions;
}

AnimationEvent
Animation::event(int index) const
{
    Assert(index >= 0 && index < events.count());
    return events[index];
}

void
Animation::setEvent(int index, const AnimationEvent &event)
{
    Assert(index >= 0 && index < events.count());
    events[index] = event;
    qSort(events);
}

void
Animation::addEvent(const AnimationEvent &event)
{
    events.append(event);
    qSort(events);
}

void
Animation::removeEvent(int index)
{
    Assert(index >= 0 && index < events.count());
    events.remove(index);
}

QVector<float>
Animation::eventPositions() const
{
    QVector<float> positions;
    foreach(const AnimationEvent &event, events)
    {
        positions.append(event.details.position);
    }

    return positions;
}

int
Animation::indexForNewKeyFramePosition(float position) const
{
    for(int i = 0; i < keys.count(); ++i)
    {
        if(position < keys[i].position())
        {
            return i;
        }
    }

    return keys.count();
}

int
Animation::indexForNewEventPosition(float position) const
{
    for(int i = 0; i < events.count(); ++i)
    {
        if(position < events[i].details.position)
        {
            return i;
        }
    }

    return events.count();
}

void
Animation::addJoints(int count)
{
    for(int i = 0; i < keys.count(); ++i)
    {
        keys[i].addJoints(count);
    }
}

void
Animation::removeJoints(const QVector<int> &indices)
{
    for(int i = 0; i < keys.count(); ++i)
    {
        keys[i].removeJoints(indices);
    }
}

void
Animation::write(QDataStream &stream, int version) const
{
    stream << det.name << det.frames << det.duration;

    stream << keys.count();
    for(int i = 0; i < keys.count(); ++i)
    {
        keys[i].write(stream, version);
    }

    stream << events.count();
    for(int i = 0; i < events.count(); ++i)
    {
        stream << events[i].details.position << events[i].details.data;
    }
}

void
Animation::read(QDataStream &stream, int version)
{
    stream >> det.name >> det.frames >> det.duration;

    int count;
    stream >> count;

    keys.clear();
    for(int i = 0; i < count; ++i)
    {
        KeyFrame key;
        key.read(stream, version);

        keys.append(key);
    }

    events.clear();

    if(version >= 4)
    {
        stream >> count;

        for(int i = 0; i < count; ++i)
        {
            AnimationEvent event;
            stream >> event.details.position >> event.details.data;

            events.append(event);
        }
    }
}
