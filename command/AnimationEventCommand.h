#ifndef ANIMATIONEVENTCOMMAND_H
#define ANIMATIONEVENTCOMMAND_H

#include "Command.h"

#include "model/Animation.h"

class AnimationEventCommand : public Command
{
public:
    AnimationEventCommand(const QString &name, Model *model, QObject *parent = 0);

    virtual
    QString
    name() const;

    virtual
    bool
    isValid() const;

    virtual
    bool
    doesModifyPersistentState() const;

    virtual
    void
    undo();

    virtual
    void
    redo();

    void
    add(int animation, const AnimationEvent &event);

    void
    remove(int animation, int index);

    void
    modify(int animation, int index, const AnimationEvent &event);

private:
    QString commandName;

    int anim;
    StoredAnimationEvent stored;

    bool valid;
};

#endif // ANIMATIONEVENTCOMMAND_H
