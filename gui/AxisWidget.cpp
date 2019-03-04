#include "AxisWidget.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtGui/QPainter>
#include <QtGui/QLinearGradient>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QMenu>
#include <QtGui/QDoubleValidator>

#include "settings/Settings.h"

#include "style/Style.h"

#include "gui/PanelGroup.h"
#include "gui/PanelLineEdit.h"
#include "gui/PanelButton.h"
#include "gui/PanelComboBox.h"

#include "model/Model.h"

AxisWidget::AxisWidget(Model *model, const QString &actionText, bool transformOptions, bool cursorIcon, QWidget *parent)
    : QWidget(parent),
      model(model),
      optionsCombo(0)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(2);

    addRow(layout, "X", 0);
    addRow(layout, "Y", 1);
    addRow(layout, "Z", 2);

    actionRow = new PanelGroup();

    actionButton = new PanelButton(actionText);
    actionButton->setMinimumWidth(60);
    actionButton->setFrame(false);

    connect(actionButton, SIGNAL(clicked()), SIGNAL(actionClicked()));

    actionRow->addWidget(actionButton);

    if(transformOptions)
    {
        optionsCombo = new PanelComboBox();
        optionsCombo->addItem("Abs", Absolute);
        optionsCombo->addItem("Rel", Relative);

        optionsCombo->setFrame(false);

        actionRow->addWidget(optionsCombo);
    }

    buttonLayout = new QHBoxLayout();
    buttonLayout->setMargin(0);

    buttonLayout->addStretch();
    buttonLayout->addWidget(actionRow);

    layout->addLayout(buttonLayout);

    connect(model, SIGNAL(axisLocked(int,bool)), SLOT(enableAxis(int,bool)));
    connect(this, SIGNAL(enabled(int,bool)), model, SLOT(lockAxis(int,bool)));
}

Vec3
AxisWidget::values(const Vec3 &disabledValues) const
{
    Vec3 v;

    for(int i = 0; i < 3; ++i)
    {
        if(edits[i]->isEnabled())
        {
            bool ok = true;
            v[i] = edits[i]->text().toFloat(&ok);

            if(!ok)
            {
                v[i] = edits[i]->placeholderText().toFloat();
            }
        }
        else
        {
            v[i] = disabledValues[i];
        }
    }

    return v;
}

Vec3
AxisWidget::filter() const
{
    Vec3 v;

    for(int i = 0; i < 3; ++i)
    {
        v[i] = (edits[i]->isEnabled() ? 1 : 0);
    }

    return v;
}

AxisWidget::TransformType
AxisWidget::transformType() const
{
    if(optionsCombo)
    {
        return static_cast<TransformType>(optionsCombo->currentData().toInt());
    }

    return Absolute;
}

void
AxisWidget::addWidget(QWidget *widget)
{
    buttonLayout->insertWidget(0, widget);
}

void
AxisWidget::setPlaceholder(int value)
{
    for(int i = 0; i < 3; ++i)
    {
        edits[i]->setPlaceholderText(QString::number(value, 'f', 2));
    }
}

void
AxisWidget::loadSettings(Settings *settings)
{
    edits[0]->setText(settings->child("x")->value().toString());
    edits[1]->setText(settings->child("y")->value().toString());
    edits[2]->setText(settings->child("z")->value().toString());

    if(optionsCombo)
    {
        optionsCombo->setCurrentData(settings->child("transform")->value());
    }
}

void
AxisWidget::saveSettings(Settings *settings) const
{
    settings->add("x", edits[0]->text());
    settings->add("y", edits[1]->text());
    settings->add("z", edits[2]->text());

    if(optionsCombo)
    {
        settings->add("transform", optionsCombo->currentData().toInt());
    }
}

void
AxisWidget::enableAxis(int axis, bool state)
{
    for(int i = 0; i < 3; ++i)
    {
        if(buttons[i]->data().toInt() == axis)
        {
            buttons[i]->setDown(state);
        }
    }
}

void
AxisWidget::axisEnabled(bool state)
{
    emit enabled(static_cast<PanelButton*>(sender())->data().toInt(), state);
}

void
AxisWidget::addRow(QVBoxLayout *layout, const QString &text, int index)
{
    PanelGroup *row = new PanelGroup();

    PanelLineEdit *edit = new PanelLineEdit();
    edit->setPlaceholderText("0.00");
    edit->setValidator(new QDoubleValidator(edit));

    edits.append(edit);

    row->setMaximumHeight(edit->sizeHint().height() + 2);

    PanelButton *button = new PanelButton(text);
    button->setMinimumWidth(button->sizeHint().height());
    button->setFrame(false);
    button->setCheckable(true);
    button->setData(index);
    button->setMinimumHeight(row->maximumHeight() - 2);

    buttons.append(button);

    connect(button, SIGNAL(toggled(bool)), edit, SLOT(setEnabled(bool)));
    connect(button, SIGNAL(toggled(bool)), SLOT(axisEnabled(bool)));

    row->addWidget(button);
    row->addWidget(edit);

    layout->addWidget(row);
}
