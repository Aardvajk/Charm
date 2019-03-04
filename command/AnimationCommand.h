#ifndef ANIMATIONCOMMAND_H
#define ANIMATIONCOMMAND_H

#include "Command.h"

#include "model/Animation.h"

class AnimationCommand : public Command
{
public:
    AnimationCommand(const QString &name, Model *model, QObject *parent = 0);

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
    add(const Animation &animation);

    void
    modify(int index, const Animation &animation);

    void
    remove(int index);

private:
    QString commandName;
    StoredAnimation stored;
};

#endif // ANIMATIONCOMMAND_H
