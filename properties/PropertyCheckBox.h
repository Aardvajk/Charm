#ifndef PROPERTYCHECKBOX_H
#define PROPERTYCHECKBOX_H

#include <QtWidgets/QCheckBox>

class Property;

class PropertyCheckBox : public QCheckBox
{
    Q_OBJECT

public:
    PropertyCheckBox(Property *property, QWidget *widget = 0);

private slots:
    void
    internalToggled(bool state);

private:
    Property *prop;
};

#endif // PROPERTYCHECKBOX_H
