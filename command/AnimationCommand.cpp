#include "AnimationCommand.h"

#include "debug/Assert.h"

#include "model/Model.h"
#include "model/ModelData.h"

AnimationCommand::AnimationCommand(const QString &name, Model *model, QObject *parent)
    : Command(model, parent),
      commandName(name)
{
}

QString
AnimationCommand::name() const
{
    return commandName;
}

bool
AnimationCommand::isValid() const
{
    return true;
}

bool
AnimationCommand::doesModifyPersistentState() const
{
    return true;
}

void
AnimationCommand::undo()
{
    if(stored.old.isNull())
    {
        Assert(stored.index >= 0 && stored.index < modelData()->animations.count());

        modelData()->animations.remove(stored.index);
        modelData()->model()->changeAnimations();
    }
    else if(stored.value.isNull())
    {
        Assert(stored.index >= 0 && stored.index <= modelData()->animations.count());

        modelData()->animations.insert(stored.index, stored.old);
        modelData()->model()->changeAnimations();
    }
    else
    {
        Assert(stored.index >= 0 && stored.index < modelData()->animations.count());

        modelData()->animations[stored.index] = stored.old;
        modelData()->model()->changeAnimation(stored.index);
    }
}

void
AnimationCommand::redo()
{
    if(stored.old.isNull())
    {
        modelData()->animations.append(stored.value);
        modelData()->model()->changeAnimations();
    }
    else if(stored.value.isNull())
    {
        Assert(stored.index >= 0 && stored.index < modelData()->animations.count());

        modelData()->animations.remove(stored.index);
        modelData()->model()->changeAnimations();
    }
    else
    {
        Assert(stored.index >= 0 && stored.index < modelData()->animations.count());

        modelData()->animations[stored.index] = stored.value;
        modelData()->model()->changeAnimation(stored.index);
    }
}

void
AnimationCommand::add(const Animation &animation)
{
    stored = StoredAnimation(modelData()->animations.count(), Animation(), animation);

    modelData()->animations.append(animation);
    modelData()->model()->changeAnimations();
}

void
AnimationCommand::modify(int index, const Animation &animation)
{
    Assert(index >= 0 && index < modelData()->animations.count());
    stored = StoredAnimation(index, modelData()->animations[index], animation);

    modelData()->animations[index] = animation;
    modelData()->model()->changeAnimation(index);
}

void
AnimationCommand::remove(int index)
{
    Assert(index >= 0 && index < modelData()->animations.count());
    stored = StoredAnimation(index, modelData()->animations[index], Animation());

    modelData()->animations.remove(index);
    modelData()->model()->changeAnimations();
}

