#include "PropertyPanel.h"
#include "Property.h"
#include "PropertyLineEdit.h"
#include "PropertyCheckBox.h"

#include "gui/PanelLineEdit.h"

#include "common/StrictDoubleValidator.h"

#include "maths/Maths.h"

#include <QtWidgets/QFormLayout>
#include <QtGui/QIntValidator>

PropertyPanel::PropertyPanel(QWidget *parent)
    : QWidget(parent)
{
    formLayout = new QFormLayout(this);
    formLayout->setMargin(0);
    formLayout->setSpacing(2);

    formLayout->setHorizontalSpacing(8);
}

void
PropertyPanel::setProperties(const QVector<Property*> &values)
{
    while(formLayout->count() > 0)
    {
        QLayoutItem *item = formLayout->takeAt(0);

        delete item->widget();
        delete item;
    }

    foreach(Property *property, values)
    {
        if(property->type() == Property::Int || property->type() == Property::Float)
        {
            formLayout->addRow(property->name(), new PropertyLineEdit(property));
        }
        else if(property->type() == Property::Bool)
        {
            formLayout->addRow(property->name(), new PropertyCheckBox(property));
        }
    }
}
