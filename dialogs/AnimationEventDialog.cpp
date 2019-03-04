#include "AnimationEventDialog.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtGui/QDoubleValidator>

#include "gui/PanelSeparator.h"

AnimationEventDialog::AnimationEventDialog(const AnimationEventDetails &details, QWidget *parent)
    : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint),
      det(details)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QFormLayout *form = new QFormLayout();
    form->setMargin(0);

    QDoubleValidator *validator = new QDoubleValidator();
    validator->setNotation(QDoubleValidator::StandardNotation);
    validator->setBottom(0);

    frameEdit = new QLineEdit();
    frameEdit->setValidator(validator);
    frameEdit->setText(QString::number(det.position));

    frameEdit->setPlaceholderText("0");

    form->addRow("Frame", frameEdit);

    dataEdit = new QPlainTextEdit(details.data);
    dataEdit->setMinimumWidth(240);

    form->addRow("Data", dataEdit);

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

    setWindowTitle(tr("Animation Event"));

    okButton->setEnabled(false);

    connect(frameEdit, SIGNAL(textChanged(QString)), SLOT(enableOk()));
    connect(dataEdit, SIGNAL(textChanged()), SLOT(enableOk()));
}

void
AnimationEventDialog::okClicked()
{
    det.position = frameEdit->text().toFloat();
    det.data = dataEdit->toPlainText();

    accept();
}

void
AnimationEventDialog::enableOk()
{
    okButton->setEnabled(!qFuzzyCompare(frameEdit->text().toFloat(), det.position) || dataEdit->toPlainText() != det.data);
}

