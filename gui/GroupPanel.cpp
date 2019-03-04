#include "GroupPanel.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>

#include "debug/Assert.h"

#include "model/Model.h"

#include "gui/PanelListBox.h"
#include "gui/PanelGroup.h"
#include "gui/PanelButton.h"

#include "command/GroupCommand.h"
#include "command/SelectCommand.h"

#include "dialogs/GroupDialog.h"

GroupPanel::GroupPanel(Model *model, QWidget *parent)
    : QWidget(parent),
      model(model)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    layout->addWidget(new QLabel("Groups"));
    layout->addSpacing(6);

    list = new PanelListBox();
    list->setDrawSelection(true);
    list->setMaximumHeight(list->heightForItems(6));

    layout->addWidget(list);

    selectButton = new PanelButton("Select");
    selectButton->setFrame(false);
    connect(selectButton, SIGNAL(clicked()), SLOT(select()));

    newButton = new PanelButton("New");
    newButton->setFrame(false);
    connect(newButton, SIGNAL(clicked()), SLOT(addNew()));

    addButton = new PanelButton("Add");
    addButton->setFrame(false);
    connect(addButton, SIGNAL(clicked()), SLOT(add()));

    editButton = new PanelButton("Edit");
    editButton->setFrame(false);
    connect(editButton, SIGNAL(clicked()), SLOT(edit()));

    delButton = new PanelButton("Delete");
    delButton->setFrame(false);
    connect(delButton, SIGNAL(clicked()), SLOT(del()));

    selectButton->setEnabled(false);
    newButton->setEnabled(false);
    addButton->setEnabled(false);
    editButton->setEnabled(false);
    delButton->setEnabled(false);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setMargin(0);

    PanelGroup *selectGroup = new PanelGroup();

    selectGroup->addWidget(selectButton);

    PanelGroup *editGroup = new PanelGroup();

    editGroup->addWidget(newButton);
    editGroup->addWidget(addButton);
    editGroup->addWidget(editButton);
    editGroup->addWidget(delButton);

    buttonLayout->addWidget(selectGroup);
    buttonLayout->addStretch();
    buttonLayout->addWidget(editGroup);

    layout->addSpacing(2);
    layout->addLayout(buttonLayout);

    connect(model, SIGNAL(selectionChanged()), SLOT(enableButtons()));
    connect(model, SIGNAL(groupsChanged()), SLOT(groupsChanged()));

    connect(list, SIGNAL(currentChanged(int)), SLOT(enableButtons()));
    connect(list, SIGNAL(itemDoubleClicked(int)), SLOT(select()));
}

void
GroupPanel::groupsChanged()
{
    int current = list->currentItem();

    list->clear();
    for(int i = 0; i < model->groupCount(); ++i)
    {
        list->addItem(model->group(i).details.name);
        if(model->groupCount(model->group(i).id) == 0)
        {
            QFont font = list->itemFont(i);
            font.setItalic(true);

            list->setItemFont(i, font);
            list->setItemColor(i, Qt::darkGray);
        }
    }

    if(list->count())
    {
        if(current >= 0 && current < list->count())
        {
            list->setCurrentItem(current);
        }
    }

    enableButtons();
}

void
GroupPanel::select()
{
    int index = list->currentItem();
    Assert(index >= 0 && index < model->groupCount());

    int id = model->group(index).id;

    QVector<int> verts;
    for(int i = 0; i < model->vertexCount(); ++i)
    {
        if(!model->vertex(i).selected && model->vertex(i).group == id) verts.append(i);
    }

    QVector<int> faces;
    for(int i = 0; i < model->faceCount(); ++i)
    {
        if(!model->face(i).selected && model->face(i).group == id) faces.append(i);
    }

    QVector<int> joints;
    for(int i = 0; i < model->jointCount(); ++i)
    {
        if(!model->joint(i).selected && model->joint(i).group == id) joints.append(i);
    }

    SelectCommand *command = new SelectCommand(model, this);
    command->selectVertices(verts);
    command->selectFaces(faces);
    command->selectJoints(joints);

    model->endCommand(command);
}

void
GroupPanel::addNew()
{
    GroupCommand *command = new GroupCommand("Add Group", model, this);
    command->add(Group(model->nextGroupId()));

    model->endCommand(command);

    list->setCurrentItem(list->count() - 1);

    enableButtons();
}

void
GroupPanel::add()
{
    int index = list->currentItem();
    Assert(index >= 0 && index < model->groupCount());

    GroupCommand *command = new GroupCommand("Add To Group", model, this);
    command->addToGroup(model->group(index).id);

    model->endCommand(command);
}

void
GroupPanel::edit()
{
    int index = list->currentItem();
    Assert(index >= 0 && index < model->groupCount());

    GroupDialog dialog(model->group(index).details, this);

    if(dialog.exec() == QDialog::Accepted)
    {
        Group group = model->group(index);
        group.details = dialog.details();

        GroupCommand *command = new GroupCommand("Edit Group", model, this);
        command->modify(index, group);

        model->endCommand(command);
    }
}

void
GroupPanel::del()
{
    Assert(list->currentItem() >= 0 && list->currentItem() < model->groupCount());

    int id = model->group(list->currentItem()).id;

    GroupCommand *command = new GroupCommand("Delete Group", model, this);
    command->remove(list->currentItem());

    model->endCommand(command);
    model->removeGroupId(id);
}

void
GroupPanel::enableButtons()
{
    bool any = list->currentItem() != -1;

    selectButton->setEnabled(any);
    editButton->setEnabled(any);
    delButton->setEnabled(any);

    newButton->setEnabled(model->anySelection());
    addButton->setEnabled(model->anySelection() && any);
}
