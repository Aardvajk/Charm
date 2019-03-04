#ifndef KEYFRAMECOMMAND_H
#define KEYFRAMECOMMAND_H

#include "Command.h"

#include "model/KeyFrame.h"

class KeyFrameCommand : public Command
{
public:
    KeyFrameCommand(const QString &name, Model *model, QObject *parent = 0);

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
    add(int animation, const KeyFrame &key);

    void
    remove(int animation, int index);

    void
    get(int animation, int index);

private:
    QString commandName;

    int anim;
    StoredKeyFrame stored;

    bool valid;
};

#endif // KEYFRAMECOMMAND_H
