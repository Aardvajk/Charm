#ifndef MOVECURSORCOMMAND_H
#define MOVECURSORCOMMAND_H

#include "Command.h"

#include "maths/Vec3.h"

class MoveCursorCommand : public Command
{
public:
    MoveCursorCommand(Model *model, QObject *parent = 0);

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
    begin(const Vec3 &position);

    void
    update(const Vec3 &position);

    void
    finalise();

private:
    StoredPosition pos;
};

#endif // MOVECURSORCOMMAND_H
