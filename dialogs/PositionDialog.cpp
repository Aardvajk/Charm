#include "PositionDialog.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtGui/QDoubleValidator>

#include "maths/Maths.h"

#include "gui/PanelSeparator.h"

PositionDialog::PositionDialog(const Vec3 &pos, QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint),
      pos(pos)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QFormLayout *form = new QFormLayout();
    form->setMargin(0);

    addRow(form, "X", pos.x);
    addRow(form, "Y", pos.y);
    addRow(form, "Z", pos.z);

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

    setWindowTitle(tr("Vertex Position"));

    okButton->setEnabled(false);
}

Vec3
PositionDialog::value() const
{
    return Vec3(edits[0]->text().toFloat(), edits[1]->text().toFloat(), edits[2]->text().toFloat());
}

void
PositionDialog::okClicked()
{
    accept();
}

void
PositionDialog::enableOk()
{
    okButton->setEnabled(pos != value());
}

void
PositionDialog::addRow(QFormLayout *layout, const QString &label, float pos)
{
    QLineEdit *edit = new QLineEdit();
    edit->setValidator(new QDoubleValidator(edit));

    edit->setText(formatFloat(pos));

    connect(edit, SIGNAL(textChanged(QString)), SLOT(enableOk()));

    layout->addRow(label, edit);
    edits.push_back(edit);
}
