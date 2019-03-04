#include "KeyFrameCommand.h"

#include "debug/Assert.h"

#include "model/Model.h"
#include "model/ModelData.h"

KeyFrameCommand::KeyFrameCommand(const QString &name, Model *model, QObject *parent)
    : Command(model, parent),
      commandName(name),
      valid(true)
{
}

QString
KeyFrameCommand::name() const
{
    return commandName;
}

bool
KeyFrameCommand::isValid() const
{
    return valid;
}

bool
KeyFrameCommand::doesModifyPersistentState() const
{
    return true;
}

void
KeyFrameCommand::undo()
{
    Assert(anim >= 0 && anim < modelData()->animations.count());

    if(stored.old.isNull())
    {
        Assert(stored.index >= 0 && stored.index < modelData()->animations[anim].keyFrameCount());
        modelData()->animations[anim].removeKeyFrame(stored.index);
    }
    else if(stored.value.isNull())
    {
        modelData()->animations[anim].addKeyFrame(stored.old);
    }
    else
    {
        stored.old.toModelData(modelData());
    }

    modelData()->model()->changeAnimation(anim);
}

void
KeyFrameCommand::redo()
{
    Assert(anim >= 0 && anim < modelData()->animations.count());

    if(stored.old.isNull())
    {
        modelData()->animations[anim].addKeyFrame(stored.value);
    }
    else if(stored.value.isNull())
    {
        Assert(stored.index >= 0 && stored.index < modelData()->animations[anim].keyFrameCount());
        modelData()->animations[anim].removeKeyFrame(stored.index);
    }
    else
    {
        stored.value.toModelData(modelData());
    }

    modelData()->model()->changeAnimation(anim);
}

void
KeyFrameCommand::add(int animation, const KeyFrame &key)
{
    Assert(animation >= 0 && animation < modelData()->animations.count());

    anim = animation;
    stored = StoredKeyFrame(modelData()->animations[anim].indexForNewKeyFramePosition(key.position()), KeyFrame(), key);

    modelData()->animations[anim].addKeyFrame(key);
    modelData()->model()->changeAnimation(anim);

    modelData()->model()->setPoseChangedFromKeyFrame(false);
}

void
KeyFrameCommand::remove(int animation, int index)
{
    Assert(animation >= 0 && animation < modelData()->animations.count());
    Assert(index >= 0 && index < modelData()->animations[animation].keyFrameCount());

    anim = animation;
    stored = StoredKeyFrame(index, modelData()->animations[anim].keyFrame(index), KeyFrame());

    modelData()->animations[anim].removeKeyFrame(index);
    modelData()->model()->changeAnimation(anim);
}

void
KeyFrameCommand::get(int animation, int index)
{
    Assert(animation >= 0 && animation < modelData()->animations.count());
    Assert(index >= 0 && index < modelData()->animations[animation].keyFrameCount());

    anim = animation;
    stored = StoredKeyFrame(index, KeyFrame(modelData()->model()), modelData()->animations[anim].keyFrame(index));

    modelData()->animations[anim].keyFrame(index).toModelData(modelData());
    modelData()->model()->change();

    if(!modelData()->model()->poseChangedFromKeyFrame())
    {
        valid = false;
    }

    modelData()->model()->setPoseChangedFromKeyFrame(false);
}
