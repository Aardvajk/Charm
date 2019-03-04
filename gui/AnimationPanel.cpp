#include "AnimationPanel.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>

#include "debug/Assert.h"

#include "model/Model.h"

#include "gui/PanelListBox.h"
#include "gui/PanelGroup.h"
#include "gui/PanelButton.h"

#include "dialogs/AnimationDialog.h"

#include "command/AnimationCommand.h"

AnimationPanel::AnimationPanel(Model *model, QWidget *parent)
    : QWidget(parent),
      model(model)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    layout->addWidget(new QLabel("Animation"));
    layout->addSpacing(6);

    list = new PanelListBox();
    list->setDrawSelection(true);
    list->setMaximumHeight(list->heightForItems(6));

    connect(list, SIGNAL(currentChanged(int)), SLOT(currentIndexChanged(int)));

    layout->addWidget(list);

    PanelButton *addButton = new PanelButton("Add");
    addButton->setFrame(false);
    connect(addButton, SIGNAL(clicked()), SLOT(add()));

    dupButton = new PanelButton("Dup");
    dupButton->setFrame(false);
    connect(dupButton, SIGNAL(clicked()), SLOT(duplicate()));

    editButton = new PanelButton("Edit");
    editButton->setFrame(false);
    connect(editButton, SIGNAL(clicked()), SLOT(edit()));

    delButton = new PanelButton("Delete");
    delButton->setFrame(false);
    connect(delButton, SIGNAL(clicked()), SLOT(del()));

    dupButton->setEnabled(false);
    editButton->setEnabled(false);
    delButton->setEnabled(false);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setMargin(0);

    PanelGroup *group = new PanelGroup();

    group->addWidget(addButton);
    group->addWidget(dupButton);
    group->addWidget(editButton);
    group->addWidget(delButton);

    buttonLayout->addStretch();
    buttonLayout->addWidget(group);

    layout->addSpacing(2);
    layout->addLayout(buttonLayout);

    connect(model, SIGNAL(animationsChanged()), SLOT(animationsChanged()));
    connect(model, SIGNAL(animationChanged(int)), SLOT(animationChanged(int)));
}

void
AnimationPanel::animationsChanged()
{
    int current = list->currentItem();

    list->clear();
    for(int i = 0; i < model->animationCount(); ++i)
    {
        list->addItem(model->animation(i).name());
    }

    if(list->count())
    {
        if(current < 0) current = 0;
        if(current >= list->count()) current = list->count() - 1;

        list->setCurrentItem(current);
    }

    enableButtons();
}

void
AnimationPanel::animationChanged(int index)
{
    Assert(index >= 0 && index < list->count() && index < model->animationCount());
    list->setItemText(index, model->animation(index).name());
}

void
AnimationPanel::currentIndexChanged(int index)
{
    enableButtons();
    emit currentChanged(index);
}

void
AnimationPanel::add()
{
    AnimationDialog dialog(AnimationDetails("New Animation"));

    if(dialog.exec() == QDialog::Accepted)
    {
        AnimationCommand *command = new AnimationCommand("Add Animation", model, this);
        command->add(Animation(dialog.details()));

        model->endCommand(command);

        list->setCurrentItem(list->count() - 1);
    }
}

void
AnimationPanel::duplicate()
{
    int index = list->currentItem();
    Assert(index >= 0 && index < model->animationCount());

    AnimationDialog dialog(model->animation(index).details(), AnimationDialog::TrackChanges, this);

    if(dialog.exec() == QDialog::Accepted)
    {
        Animation animation = model->animation(index);
        animation.setDetails(dialog.details());

        AnimationCommand *command = new AnimationCommand("Duplicate Animation", model, this);
        command->add(animation);

        model->endCommand(command);

        list->setCurrentItem(list->count() - 1);
    }
}

void
AnimationPanel::edit()
{
    int index = list->currentItem();
    Assert(index >= 0 && index < model->animationCount());

    AnimationDialog dialog(model->animation(index).details(), AnimationDialog::TrackChanges, this);

    if(dialog.exec() == QDialog::Accepted)
    {
        Animation animation = model->animation(index);
        animation.setDetails(dialog.details());

        AnimationCommand *command = new AnimationCommand("Edit Animation", model, this);
        command->modify(index, animation);

        model->endCommand(command);
    }
}

void
AnimationPanel::del()
{
    Assert(list->currentItem() != -1);

    AnimationCommand *command = new AnimationCommand("Delete Animation", model, this);
    command->remove(list->currentItem());

    model->endCommand(command);
}

void
AnimationPanel::enableButtons()
{
    bool any = list->currentItem() != -1;

    dupButton->setEnabled(any);
    editButton->setEnabled(any);
    delButton->setEnabled(any);
}
