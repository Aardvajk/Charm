#ifndef PALETTECOMMAND_H
#define PALETTECOMMAND_H

#include "Command.h"

#include "common/StoredItem.h"

class PaletteCommand : public Command
{
public:
    PaletteCommand(const QString &name, Model *model, QObject *parent = 0);

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
    add(const QColor &color);

    void
    edit(int index, const QColor &color);

    void
    del(int index);

private:
    QString commandName;
    StoredColor stored;
};

#endif // PALETTECOMMAND_H
