#include "PaletteCommand.h"

#include "debug/Assert.h"

#include "model/Model.h"
#include "model/ModelData.h"

PaletteCommand::PaletteCommand(const QString &name, Model *model, QObject *parent)
    : Command(model, parent),
      commandName(name)
{
}

QString
PaletteCommand::name() const
{
    return commandName;
}

bool
PaletteCommand::isValid() const
{
    return true;
}

bool
PaletteCommand::doesModifyPersistentState() const
{
    return true;
}

void
PaletteCommand::undo()
{
    if(!stored.old.isValid())
    {
        Assert(stored.index >= 0 && stored.index < modelData()->palette.count());
        modelData()->palette.remove(stored.index);
    }
    else if(!stored.value.isValid())
    {
        modelData()->palette.insert(stored.index, stored.old);
    }
    else
    {
        Assert(stored.index >= 0 && stored.index < modelData()->palette.count());
        modelData()->palette[stored.index] = stored.old;
    }

    modelData()->model()->changePalette();
}

void
PaletteCommand::redo()
{
    if(!stored.old.isValid())
    {
        modelData()->palette.append(stored.value);
    }
    else if(!stored.value.isValid())
    {
        Assert(stored.index >= 0 && stored.index < modelData()->palette.count());
        modelData()->palette.remove(stored.index);
    }
    else
    {
        Assert(stored.index >= 0 && stored.index < modelData()->palette.count());
        modelData()->palette[stored.index] = stored.value;
    }

    modelData()->model()->changePalette();
}

void
PaletteCommand::add(const QColor &color)
{
    stored = StoredColor(modelData()->palette.count(), QColor(), color);

    modelData()->palette.append(color);

    modelData()->model()->changePalette();
}

void
PaletteCommand::edit(int index, const QColor &color)
{
    stored = StoredColor(index, modelData()->palette[index], color);

    modelData()->palette[index] = color;

    modelData()->model()->changePalette();
}

void
PaletteCommand::del(int index)
{
    stored = StoredColor(index, modelData()->palette[index], QColor());

    modelData()->palette.remove(index);

    modelData()->model()->changePalette();
}
