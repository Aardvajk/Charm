#ifndef AXISLOCKWIDGET_H
#define AXISLOCKWIDGET_H

#include <QtWidgets/QWidget>

#include "common/VariantSet.h"

class ActionList;
class Model;
class ToggleBox;

class AxisLockWidget : public QWidget
{
    Q_OBJECT

public:
    AxisLockWidget(ActionList *actions, Model *model, QWidget *parent = 0);

    VariantSet
    values() const;

    void
    setValues(const VariantSet &values);

signals:
    void
    changed(int axis, bool state);

public slots:
    void
    enableAxis(int axis, bool state);

private slots:
    void
    buttonChanged(const QVariant &value, bool state);

    void
    actionTriggered();

private:
    void
    createAction(ActionList *actions, const QString &key, int data);

    ToggleBox *box;
};

#endif // AXISLOCKWIDGET_H
