#include "PropertyLineEdit.h"
#include "Property.h"

#include "maths/Maths.h"

#include "common/StrictDoubleValidator.h"
#include "common/StrictIntValidator.h"

PropertyLineEdit::PropertyLineEdit(Property *property, QWidget *parent)
    : PanelLineEdit(parent),
      prop(property)
{
    setFrame(true);

    switch(prop->type())
    {
        case Property::Int  : setText(QString::number(prop->value().toInt())); break;
        case Property::Float: setText(formatFloat(prop->value().toFloat())); break;

        default: break;
    }

    if(prop->type() == Property::Float)
    {
        StrictDoubleValidator *v = new StrictDoubleValidator(3, this);

        if(prop->min().isValid()) v->setBottom(prop->min().toFloat());
        if(prop->max().isValid()) v->setTop(prop->max().toFloat());

        setValidator(v);
    }
    else if(prop->type() == Property::Int)
    {
        StrictIntValidator *v = new StrictIntValidator(this);

        if(prop->min().isValid()) v->setBottom(prop->min().toInt());
        if(prop->max().isValid()) v->setTop(prop->max().toInt());

        setValidator(v);
    }

    baseTextColor = palette().color(QPalette::Text);

    connect(this, SIGNAL(textChanged(QString)), SLOT(valueChanged(QString)));
}

void
PropertyLineEdit::valueChanged(const QString &value)
{
    switch(prop->type())
    {
        case Property::Int  : prop->setValue(value.toInt()); break;
        case Property::Float: prop->setValue(value.toFloat()); break;

        default: break;
    }

    if(validator())
    {
        QString temp = value;
        int pos = 0;

        QValidator::State state = validator()->validate(temp, pos);

        QPalette pal = palette();

        if(state != QValidator::Acceptable)
        {
            pal.setColor(QPalette::Text, Qt::red);
        }
        else
        {
            pal.setColor(QPalette::Text, baseTextColor);
        }

        setPalette(pal);
    }
}
