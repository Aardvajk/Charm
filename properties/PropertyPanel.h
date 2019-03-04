#ifndef PROPERTYPANEL_H
#define PROPERTYPANEL_H

#include <QtWidgets/QWidget>
#include <QtCore/QVector>

class Property;
class QFormLayout;

class PropertyPanel : public QWidget
{
    Q_OBJECT

public:
    PropertyPanel(QWidget *parent = 0);

    void
    setProperties(const QVector<Property*> &values);

private:
    QFormLayout *formLayout;
};

#endif // PROPERTYPANEL_H
