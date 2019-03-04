#include "JointDialog.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>

#include "model/Model.h"

#include "gui/PanelSeparator.h"

JointDialog::JointDialog(Model *model, int joint, QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint),
      det(model->joint(joint).details)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QFormLayout *form = new QFormLayout();
    form->setMargin(0);

    nameEdit = new QLineEdit(det.name);
    nameEdit->setMinimumWidth(240);
    nameEdit->selectAll();

    nameEdit->setPlaceholderText("New Joint");

    form->addRow("Name", nameEdit);

    parentCombo = new QComboBox();
    parentCombo->addItem("None", -1);

    QFont font = parentCombo->font();
    font.setItalic(true);

    parentCombo->setItemData(0, font, Qt::FontRole);

    for(int i = 0; i < model->jointCount(); ++i)
    {
        if(i != joint && !model->isJointChild(joint, i))
        {
            parentCombo->addItem(model->joint(i).details.name, i);
        }
    }

    for(int i = 0; i < parentCombo->count(); ++i)
    {
        if(parentCombo->itemData(i).toInt() == det.parent)
        {
            parentCombo->setCurrentIndex(i);
            break;
        }
    }

    form->addRow("Parent", parentCombo);

    pairedCombo = new QComboBox();
    pairedCombo->addItem("None", -1);

    pairedCombo->setItemData(0, font, Qt::FontRole);

    for(int i = 0; i < model->jointCount(); ++i)
    {
        if(i != joint)
        {
            pairedCombo->addItem(model->joint(i).details.name, i);
        }
    }

    bool found = false;

    for(int i = 0; i < pairedCombo->count(); ++i)
    {
        if(pairedCombo->itemData(i).toInt() == det.paired)
        {
            pairedCombo->setCurrentIndex(i);
            found = true;

            break;
        }
    }

    if(!found)
    {
        pairedCombo->addItem("Unknown", det.paired);
        pairedCombo->setItemData(pairedCombo->count() - 1, font, Qt::FontRole);

        pairedCombo->setCurrentIndex(pairedCombo->count() - 1);
    }

    form->addRow("Paired", pairedCombo);

    layout->addLayout(form);
    layout->addWidget(new PanelSeparator());

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setMargin(0);

    okButton = new QPushButton("OK");
    okButton->setDefault(true);

    connect(okButton, SIGNAL(clicked()), SLOT(okClicked()));

    QPushButton *cancelButton = new QPushButton("Cancel");
    connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));

    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    layout->addLayout(buttonLayout);

    layout->setSizeConstraint(QLayout::SetFixedSize);

    setWindowTitle(tr("Joint"));

    okButton->setEnabled(false);

    connect(nameEdit, SIGNAL(textChanged(QString)), SLOT(enableOk()));
    connect(parentCombo, SIGNAL(currentIndexChanged(int)), SLOT(enableOk()));
    connect(pairedCombo, SIGNAL(currentIndexChanged(int)), SLOT(enableOk()));
}

void
JointDialog::okClicked()
{
    det.name = nameEdit->text();
    if(det.name.trimmed().isEmpty())
    {
        det.name = "New Joint";
    }

    det.parent = parentCombo->itemData(parentCombo->currentIndex()).toInt();
    det.paired = pairedCombo->itemData(pairedCombo->currentIndex()).toInt();

    accept();
}

void
JointDialog::enableOk()
{
    int parent = parentCombo->itemData(parentCombo->currentIndex()).toInt();
    int paired = pairedCombo->itemData(pairedCombo->currentIndex()).toInt();

    okButton->setEnabled(nameEdit->text() != det.name || parent != det.parent || paired != det.paired);
}

