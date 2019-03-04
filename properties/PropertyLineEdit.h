#ifndef PROPERTYLINEEDIT_H
#define PROPERTYLINEEDIT_H

#include "gui/PanelLineEdit.h"

class Property;

class PropertyLineEdit : public PanelLineEdit
{
    Q_OBJECT

public:
    PropertyLineEdit(Property *property, QWidget *parent = 0);

private slots:
    void
    valueChanged(const QString &value);

private:
    Property *prop;

    QColor baseTextColor;
};

#endif // PROPERTYLINEEDIT_H
