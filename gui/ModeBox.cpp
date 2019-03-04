#include "ModeBox.h"

#include "common/Mode.h"

#include "model/Model.h"

ModeBox::ModeBox(Model *model, QWidget *parent)
    : RadioBox(parent),
      model(model)
{
    for(int i = 0; i < static_cast<int>(Mode::Null); ++i)
    {
        Mode::Type mode = static_cast<Mode::Type>(i);
        addItem(Mode::toString(mode), mode);
    }

    connect(this, SIGNAL(valueChanged(QVariant)), SLOT(changed(QVariant)));
    connect(model, SIGNAL(modeChanged()), SLOT(modelChanged()));
}

void
ModeBox::changed(const QVariant &data)
{
    model->setMode(static_cast<Mode::Type>(data.toInt()));
}

void
ModeBox::modelChanged()
{
    blockSignals(true);
    setCurrentData(model->mode());
    blockSignals(false);
}
