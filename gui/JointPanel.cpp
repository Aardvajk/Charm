#include "JointPanel.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>

#include "debug/Assert.h"

#include "model/Model.h"

#include "dialogs/JointDialog.h"

#include "gui/PanelMultiListBox.h"
#include "gui/PanelButton.h"
#include "gui/PanelGroup.h"

#include "command/SelectCommand.h"
#include "command/ModifyCommand.h"
#include "command/ReorderJointCommand.h"

JointPanel::JointPanel(Model *model, QWidget *parent)
    : QWidget(parent),
      model(model),
      ignoreModelChange(false)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    layout->addWidget(new QLabel("Joints"));
    layout->addSpacing(6);

    list = new PanelMultiListBox();
    list->setMaximumHeight(list->heightForItems(6));

    connect(list, SIGNAL(currentChanged(QVector<int>)), SLOT(currentChanged(QVector<int>)));
    connect(list, SIGNAL(itemDoubleClicked(int)), SLOT(editClicked()));

    layout->addWidget(list);

    weightButton = new PanelButton("Set Weights");
    weightButton->setFrame(false);
    weightButton->setEnabled(false);

    connect(weightButton, SIGNAL(clicked()), SLOT(weightClicked()));

    editButton = new PanelButton("Edit");
    editButton->setFrame(false);
    editButton->setEnabled(false);

    editButton->setMinimumWidth(50);

    connect(editButton, SIGNAL(clicked()), SLOT(editClicked()));

    upButton = new PanelButton(" ");
    upButton->setFrame(false);
    upButton->setEnabled(false);
    upButton->setFixedWidth(16);
    upButton->setPixmap(QPixmap(":/resources/images/up.png"), QPixmap(":/resources/images/updis.png"));

    connect(upButton, SIGNAL(clicked()), SLOT(upClicked()));

    downButton = new PanelButton(" ");
    downButton->setFrame(false);
    downButton->setEnabled(false);
    downButton->setFixedWidth(16);
    downButton->setPixmap(QPixmap(":/resources/images/down.png"), QPixmap(":/resources/images/downdis.png"));

    connect(downButton, SIGNAL(clicked()), SLOT(downClicked()));

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setMargin(0);

    PanelGroup *weightGroup = new PanelGroup();
    weightGroup->addWidget(weightButton);

    PanelGroup *editGroup = new PanelGroup();
    editGroup->addWidget(upButton);
    editGroup->addWidget(downButton);
    editGroup->addWidget(editButton);

    buttonLayout->addWidget(weightGroup);
    buttonLayout->addStretch();
    buttonLayout->addWidget(editGroup);

    layout->addSpacing(2);
    layout->addLayout(buttonLayout);

    connect(model, SIGNAL(changed()), SLOT(modelChanged()));
    connect(model, SIGNAL(selectionChanged()), SLOT(modelChanged()));
}

void
JointPanel::modelChanged()
{
    if(ignoreModelChange)
    {
        return;
    }

    if(model->jointCount() == list->count())
    {
        QVector<int> selection = list->currentItems();
        bool changed = false;

        for(int i = 0; i < model->jointCount(); ++i)
        {
            if(list->itemText(i) != model->joint(i).details.name)
            {
                changed = true;
                break;
            }

            if(selection.contains(i) && !model->joint(i).selected)
            {
                changed = true;
                break;
            }

            if(!selection.contains(i) && model->joint(i).selected)
            {
                changed = true;
                break;
            }
        }

        if(!changed)
        {
            enableButtons();
            return;
        }
    }

    int scroll = list->currentScroll();

    list->blockSignals(true);
    list->clear();

    for(int i = 0; i < model->jointCount(); ++i)
    {
        list->addItem(model->joint(i).details.name);
    }

    list->setCurrentItems(model->selectedJoints());
    list->setCurrentScroll(scroll);

    list->blockSignals(false);

    enableButtons();
}

void
JointPanel::currentChanged(const QVector<int> &items)
{
    ignoreModelChange = true;

    SelectCommand *command = new SelectCommand(model, this);
    command->setJointSelection(items);

    model->endCommand(command);

    ignoreModelChange = false;

    enableButtons();
}

void
JointPanel::weightClicked()
{
    int joints = model->selectedJoints().count();

    Assert(joints > 0 && joints <= Vertex::maximumInfluences);

    float weight = 1.0f / static_cast<float>(joints);

    QVector<StoredVertex> verts;

    foreach(int i, model->selectedVertices())
    {
        Vertex v = model->vertex(i);

        for(int j = 0; j < model->selectedJoints().count(); ++j)
        {
            v.influences[j].joint = model->selectedJoints()[j];
            v.influences[j].weight = weight;
        }

        verts.append(StoredVertex(i, model->vertex(i), v));
    }

    ModifyCommand *command = new ModifyCommand("Set Weights", model, this);

    command->setVertices(verts);
    command->activate();

    model->endCommand(command);
}

void
JointPanel::upClicked()
{
    changeOrder(-1);
}

void
JointPanel::downClicked()
{
    changeOrder(1);
}

void
JointPanel::editClicked()
{
    if(list->currentItems().count() != 1)
    {
        return;
    }

    int i = list->currentItems().front();

    JointDialog dialog(model, i, this);
    if(dialog.exec() == QDialog::Accepted)
    {
        Joint joint = model->joint(i);
        joint.details = dialog.details();

        ModifyCommand *command = new ModifyCommand("Modify Joint", model, this);

        command->setJoint(StoredJoint(i, model->joint(i), joint));
        command->activate();

        model->endCommand(command);
    }
}

void
JointPanel::enableButtons()
{
    int joints = model->selectedJoints().count();
    weightButton->setEnabled(model->selectedVertices().count() && (joints > 0 && joints <= Vertex::maximumInfluences));

    bool single = list->currentItems().count() == 1;

    editButton->setEnabled(single);

    if(single)
    {
        int index = list->currentItems().front();

        upButton->setEnabled(index != 0);
        downButton->setEnabled(index != list->count() - 1);
    }
    else
    {
        upButton->setEnabled(false);
        downButton->setEnabled(false);
    }
}

void
JointPanel::changeOrder(int direction)
{
    if(list->currentItems().count() != 1)
    {
        return;
    }

    int from = list->currentItems().front();
    int to = from + direction;

    if(to < 0 || to >= model->jointCount())
    {
        return;
    }

    ReorderJointCommand *command = new ReorderJointCommand(model, this);

    command->activate(from, to);

    model->endCommand(command);
}
