#include "AnimationEventCommand.h"

#include "debug/Assert.h"

#include "model/Model.h"
#include "model/ModelData.h"

AnimationEventCommand::AnimationEventCommand(const QString &name, Model *model, QObject *parent)
    : Command(model, parent),
      commandName(name),
      valid(true)
{
}

QString
AnimationEventCommand::name() const
{
    return commandName;
}

bool
AnimationEventCommand::isValid() const
{
    return valid;
}

bool
AnimationEventCommand::doesModifyPersistentState() const
{
    return true;
}

void
AnimationEventCommand::undo()
{
    Assert(anim >= 0 && anim < modelData()->animations.count());

    if(stored.old.isNull())
    {
        Assert(stored.index >= 0 && stored.index < modelData()->animations[anim].eventCount());
        modelData()->animations[anim].removeEvent(stored.index);
    }
    else if(stored.value.isNull())
    {
        modelData()->animations[anim].addEvent(stored.old);
    }
    else
    {
        modelData()->animations[anim].setEvent(stored.index, stored.old);
    }

    modelData()->model()->changeAnimation(anim);
}

void
AnimationEventCommand::redo()
{
    Assert(anim >= 0 && anim < modelData()->animations.count());

    if(stored.old.isNull())
    {
        modelData()->animations[anim].addEvent(stored.value);
    }
    else if(stored.value.isNull())
    {
        Assert(stored.index >= 0 && stored.index < modelData()->animations[anim].eventCount());
        modelData()->animations[anim].removeEvent(stored.index);
    }
    else
    {
        modelData()->animations[anim].setEvent(stored.index, stored.value);
    }

    modelData()->model()->changeAnimation(anim);
}

void
AnimationEventCommand::add(int animation, const AnimationEvent &event)
{
    Assert(animation >= 0 && animation < modelData()->animations.count());

    anim = animation;
    stored = StoredAnimationEvent(modelData()->animations[anim].indexForNewEventPosition(event.details.position), AnimationEvent(), event);

    modelData()->animations[anim].addEvent(event);
    modelData()->model()->changeAnimation(anim);
}

void
AnimationEventCommand::remove(int animation, int index)
{
    Assert(animation >= 0 && animation < modelData()->animations.count());
    Assert(index >= 0 && index < modelData()->animations[animation].eventCount());

    anim = animation;
    stored = StoredAnimationEvent(index, modelData()->animations[anim].event(index), AnimationEvent());

    modelData()->animations[anim].removeEvent(index);
    modelData()->model()->changeAnimation(anim);
}

void
AnimationEventCommand::modify(int animation, int index, const AnimationEvent &event)
{
    Assert(animation >= 0 && animation < modelData()->animations.count());
    Assert(index >= 0 && index < modelData()->animations[animation].eventCount());

    anim = animation;
    stored = StoredAnimationEvent(index, modelData()->animations[anim].event(index), event);

    modelData()->animations[anim].setEvent(index, event);
    modelData()->model()->changeAnimation(anim);
}
