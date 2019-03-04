#include "NotSavedDialog.h"

#include <QtCore/QCoreApplication>

NotSavedDialog::NotSavedDialog(QWidget *parent)
    : QMessageBox(parent)
{
    setWindowTitle(QCoreApplication::applicationName());
    setIcon(QMessageBox::Question);

    setText("The current model has not been saved.");
    setInformativeText("Do you want to save it?");

    setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
}

int
NotSavedDialog::show(QWidget *parent)
{
    NotSavedDialog dialog(parent);

    return dialog.exec();
}
