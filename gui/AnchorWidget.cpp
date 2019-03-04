#include "AnchorWidget.h"

#include "actions/Action.h"
#include "actions/ActionList.h"

#include "gui/RadioBox.h"

#include "model/Model.h"

#include <QtWidgets/QHBoxLayout>

AnchorWidget::AnchorWidget(ActionList *actions, Model *model, QWidget *parent)
    : QWidget(parent),
      model(model)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);

    box = new RadioBox();
    box->setButtonMinimumSize(QSize(10, 20));

    for(int i = 0; i < static_cast<int>(Anchor::Null); ++i)
    {
        Anchor::Type type = static_cast<Anchor::Type>(i);
        box->addItem(Anchor::toCode(type), type);

        createAction(actions, Anchor::toString(type), QString(), type);
    }

    layout->addWidget(box);

    connect(box, SIGNAL(valueChanged(QVariant)), SLOT(anchorChanged(QVariant)));
    connect(model, SIGNAL(anchorChanged()), SLOT(modelAnchorChanged()));
}

Anchor::Type
AnchorWidget::type() const
{
    return static_cast<Anchor::Type>(box->value().toInt());
}

void
AnchorWidget::setType(Anchor::Type type)
{
    box->setCurrentData(type);
}

void
AnchorWidget::anchorChanged(const QVariant &value)
{
    model->setAnchor(static_cast<Anchor::Type>(value.toInt()));
}

void
AnchorWidget::modelAnchorChanged()
{
    box->setCurrentData(model->anchor());
}

void
AnchorWidget::actionTriggered()
{
    Action *action = static_cast<Action*>(sender());

    box->setCurrentData(action->data().toInt());
}

void
AnchorWidget::createAction(ActionList *actions, const QString &name, const QString &key, int data)
{
    Action *action = actions->add("Settings", QString("Anchor at %1").arg(name));
    action->setData(data);
    action->setShortcut(QKeySequence(key));
    connect(action, SIGNAL(triggered()), SLOT(actionTriggered()));
}
