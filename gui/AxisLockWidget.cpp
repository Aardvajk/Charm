#include "AxisLockWidget.h"

#include "actions/Action.h"
#include "actions/ActionList.h"

#include "gui/ToggleBox.h"

#include "model/Model.h"

#include <QtWidgets/QHBoxLayout>

AxisLockWidget::AxisLockWidget(ActionList *actions, Model *model, QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);

    box = new ToggleBox();

    box->addItem("X", 0);
    box->addItem("Y", 1);
    box->addItem("Z", 2);

    layout->addWidget(box);

    connect(box, SIGNAL(changed(QVariant,bool)), SLOT(buttonChanged(QVariant,bool)));

    createAction(actions, "X", 0);
    createAction(actions, "Y", 1);
    createAction(actions, "Z", 2);

    connect(model, SIGNAL(axisLocked(int,bool)), SLOT(enableAxis(int,bool)));
    connect(this, SIGNAL(changed(int,bool)), model, SLOT(lockAxis(int,bool)));
}

VariantSet
AxisLockWidget::values() const
{
    return box->values();
}

void
AxisLockWidget::setValues(const VariantSet &values)
{
    box->setValues(values);
}

void
AxisLockWidget::enableAxis(int axis, bool state)
{
    box->setValue(axis, state);
}

void
AxisLockWidget::buttonChanged(const QVariant &value, bool state)
{
    emit changed(value.toInt(), state);
}

void
AxisLockWidget::actionTriggered()
{
    Action *action = static_cast<Action*>(sender());

    box->setValue(action->data().toInt(), !box->value(action->data()));
}

void
AxisLockWidget::createAction(ActionList *actions, const QString &key, int data)
{
    Action *action = actions->add("Settings", QString("Lock %1 Axis").arg(key));
    action->setData(data);
    action->setShortcut(QKeySequence(key));
    connect(action, SIGNAL(triggered()), SLOT(actionTriggered()));
}
