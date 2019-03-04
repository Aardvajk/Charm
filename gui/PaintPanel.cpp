#include "PaintPanel.h"

#include "PaintListBox.h"
#include "PanelButton.h"
#include "PanelGroup.h"

#include "actions/Action.h"
#include "actions/ActionList.h"

#include "model/Model.h"

#include "command/PaletteCommand.h"
#include "command/ModifyCommand.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QColorDialog>

PaintPanel::PaintPanel(ActionList *actions, Model *model, QWidget *parent)
    : QWidget(parent),
      model(model)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    layout->addWidget(new QLabel("Paint"));
    layout->addSpacing(6);

    list = new PaintListBox(this);
    list->setMaximumHeight(list->heightForItems(5));

    layout->addWidget(list);

    paintAction = actions->add("Model", "Paint");
    paintAction->setShortcut(QKeySequence("C"));
    connect(paintAction, SIGNAL(triggered()), SLOT(paint()));

    paintButton = new PanelButton("Paint");
    paintButton->setFrame(false);
    connect(paintButton, SIGNAL(clicked()), paintAction, SIGNAL(triggered()));

    addButton = new PanelButton("Add");
    addButton->setFrame(false);
    connect(addButton, SIGNAL(clicked()), SLOT(add()));

    editButton = new PanelButton("Edit");
    editButton->setFrame(false);
    connect(editButton, SIGNAL(clicked()), SLOT(edit()));

    delButton = new PanelButton("Delete");
    delButton->setFrame(false);
    connect(delButton, SIGNAL(clicked()), SLOT(del()));

    paintButton->setEnabled(false);
    editButton->setEnabled(false);
    delButton->setEnabled(false);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setMargin(0);

    PanelGroup *paintGroup = new PanelGroup();

    paintGroup->addWidget(paintButton);

    PanelGroup *editGroup = new PanelGroup();

    editGroup->addWidget(addButton);
    editGroup->addWidget(editButton);
    editGroup->addWidget(delButton);

    buttonLayout->addWidget(paintGroup);
    buttonLayout->addStretch();
    buttonLayout->addWidget(editGroup);

    layout->addSpacing(2);
    layout->addLayout(buttonLayout);

    connect(model, SIGNAL(selectionChanged()), SLOT(enableButtons()));
    connect(model, SIGNAL(paletteChanged()), SLOT(paletteChanged()));

    connect(list, SIGNAL(currentChanged(int)), SLOT(enableButtons()));
}

void
PaintPanel::paletteChanged()
{
    int current = list->currentItem();

    list->clear();
    for(int i = 0; i < model->paletteCount(); ++i)
    {
        list->addItem("", QVariant::fromValue(model->palette(i)));
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
PaintPanel::enableButtons()
{
    bool any = list->currentItem() != -1;

    editButton->setEnabled(any);
    delButton->setEnabled(any);

    bool paint = any && !model->selectedFaces().isEmpty();

    paintButton->setEnabled(paint);
    paintAction->setEnabled(paint);
}

void
PaintPanel::paint()
{
    int curr = list->currentItem();
    QVector<StoredFace> faces;

    for(int i = 0; i < model->selectedFaces().count(); ++i)
    {
        Face face = model->face(model->selectedFaces()[i]);
        if(face.palette != curr)
        {
            Face old = face;
            face.palette = curr;

            faces.push_back(StoredFace(model->selectedFaces()[i], old, face));
        }
    }

    if(!faces.isEmpty())
    {
        ModifyCommand *command = new ModifyCommand("Paint Faces", model, this);

        command->setFaces(faces);
        command->activate();

        model->endCommand(command);
    }
}

void
PaintPanel::add()
{
    QColor color = fromDialog(Qt::white);
    if(color.isValid())
    {
        PaletteCommand *command = new PaletteCommand("Add Color", model, this);
        command->add(color);

        model->endCommand(command);

        list->setCurrentItem(list->count() - 1);
    }
}

void
PaintPanel::edit()
{
    int curr = list->currentItem();

    QColor color = fromDialog(model->palette(curr));
    if(color.isValid())
    {
        PaletteCommand *command = new PaletteCommand("Edit Color", model, this);
        command->edit(curr, color);

        model->endCommand(command);
    }
}

void
PaintPanel::del()
{
    PaletteCommand *command = new PaletteCommand("Delete Color", model, this);
    command->del(list->currentItem());

    model->endCommand(command);
}

QColor
PaintPanel::fromDialog(const QColor &initial)
{
    return QColorDialog::getColor(initial);
}

