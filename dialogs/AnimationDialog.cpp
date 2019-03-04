#include "AnimationDialog.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QPushButton>
#include <QtGui/QDoubleValidator>

#include "gui/PanelSeparator.h"

AnimationDialog::AnimationDialog(const AnimationDetails &details, Options options, QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint),
      det(details),
      ops(options)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QFormLayout *form = new QFormLayout();
    form->setMargin(0);

    nameEdit = new QLineEdit(det.name);
    nameEdit->setMinimumWidth(240);
    nameEdit->selectAll();

    nameEdit->setPlaceholderText("New Animation");

    form->addRow("Name", nameEdit);

    framesSpin = new QSpinBox();
    framesSpin->setMinimum(2);
    framesSpin->setMaximum(50);
    framesSpin->setValue(det.frames);

    form->addRow("Frames", framesSpin);

    QDoubleValidator *validator = new QDoubleValidator();
    validator->setNotation(QDoubleValidator::StandardNotation);
    validator->setBottom(0);

    durationEdit = new QLineEdit();
    durationEdit->setValidator(validator);
    durationEdit->setText(QString::number(det.duration));

    durationEdit->setPlaceholderText("0");

    form->addRow("Duration", durationEdit);

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

    setWindowTitle(tr("Animation"));

    if(ops & TrackChanges)
    {
        okButton->setEnabled(false);

        connect(nameEdit, SIGNAL(textChanged(QString)), SLOT(enableOk()));
        connect(framesSpin, SIGNAL(valueChanged(int)), SLOT(enableOk()));
        connect(durationEdit, SIGNAL(textChanged(QString)), SLOT(enableOk()));
    }
}

void
AnimationDialog::okClicked()
{
    det.name = nameEdit->text();
    if(det.name.trimmed().isEmpty())
    {
        det.name = "New Animation";
    }

    det.frames = framesSpin->value();
    det.duration = durationEdit->text().toFloat();

    accept();
}

void
AnimationDialog::enableOk()
{
    okButton->setEnabled(nameEdit->text() != det.name || framesSpin->value() != det.frames || !qFuzzyCompare(durationEdit->text().toFloat(), det.duration));
}
