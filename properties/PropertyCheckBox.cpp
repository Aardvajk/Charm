#include "PropertyCheckBox.h"
#include "Property.h"

PropertyCheckBox::PropertyCheckBox(Property *property, QWidget *widget)
    : QCheckBox(widget),
      prop(property)
{
    setChecked(property->value().toBool());

    connect(this, SIGNAL(toggled(bool)), SLOT(internalToggled(bool)));
}

void
PropertyCheckBox::internalToggled(bool state)
{
    prop->setValue(state);
}
