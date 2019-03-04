#include "GroupDialog.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

#include "gui/PanelSeparator.h"

GroupDialog::GroupDialog(const GroupDetails &details, QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint),
      det(details)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QFormLayout *form = new QFormLayout();
    form->setMargin(0);

    nameEdit = new QLineEdit(det.name);
    nameEdit->setMinimumWidth(240);
    nameEdit->selectAll();

    nameEdit->setPlaceholderText("New Group");

    form->addRow("Name", nameEdit);

    layout->addLayout(form);
    layout->addWidget(new PanelSeparator());

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setMargin(0);

    okButton = new QPushButton("OK");
    connect(okButton, SIGNAL(clicked()), SLOT(okClicked()));

    QPushButton *cancelButton = new QPushButton("Cancel");
    connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));

    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    layout->addLayout(buttonLayout);

    layout->setSizeConstraint(QLayout::SetFixedSize);

    setWindowTitle(tr("Group"));

    okButton->setEnabled(false);

    connect(nameEdit, SIGNAL(textChanged(QString)), SLOT(enableOk()));
}

void
GroupDialog::okClicked()
{
    det.name = nameEdit->text();
    if(det.name.trimmed().isEmpty())
    {
        det.name = "New Group";
    }

    accept();
}

void
GroupDialog::enableOk()
{
    okButton->setEnabled(nameEdit->text() != det.name);
}

