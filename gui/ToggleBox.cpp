#include "ToggleBox.h"

#include "gui/PanelButton.h"

#include <QtWidgets/QLayout>

ToggleBox::ToggleBox(QWidget *parent)
    : PanelGroup(parent)
{
}

void
ToggleBox::addItem(const QString &text, const QVariant &data)
{
    PanelButton *button = new PanelButton(text);
    button->setData(data);
    button->setFrame(false);
    button->setCheckable(true);
    button->setMinimumHeight(20);

    buttons.append(button);

    connect(button, SIGNAL(toggled(bool)), SLOT(buttonToggled(bool)));

    addWidget(button);
}

VariantSet
ToggleBox::values() const
{
    VariantSet set;
    foreach(const PanelButton *button, buttons)
    {
        if(button->down())
        {
            set.insert(button->data());
        }
    }

    return set;
}

bool
ToggleBox::value(const QVariant &item) const
{
    foreach(const PanelButton *button, buttons)
    {
        if(button->data() == item)
        {
            return button->down();
        }
    }

    return false;
}

void
ToggleBox::setValue(const QVariant &value, bool state)
{
    foreach(PanelButton *button, buttons)
    {
        if(button->data() == value)
        {
            button->setDown(state);
        }
    }
}

void
ToggleBox::setValues(const VariantSet &values)
{
    foreach(PanelButton *button, buttons)
    {
        button->setDown(values.contains(button->data()));
    }
}

void
ToggleBox::setEnabledValues(const VariantSet &values)
{
    foreach(PanelButton *button, buttons)
    {
        button->setEnabled(values.contains(button->data()));
    }
}

void
ToggleBox::buttonToggled(bool state)
{
    emit changed(static_cast<PanelButton*>(sender())->data(), state);
}
