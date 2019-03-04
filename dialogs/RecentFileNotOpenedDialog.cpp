#include "RecentFileNotOpenedDialog.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>

RecentFileNotOpenedDialog::RecentFileNotOpenedDialog(const QString &path, QWidget *parent)
    : QMessageBox(parent)
{
    setWindowTitle(QCoreApplication::applicationName());
    setIcon(QMessageBox::Question);

    setText(QString("<b>%1</b> could not be opened.").arg(QFileInfo(path).fileName()));
    setInformativeText("Do you want to remove it from the Recent Files list?");

    setStandardButtons(QMessageBox::Yes | QMessageBox::No);
}

int
RecentFileNotOpenedDialog::show(QWidget *parent, const QString &path)
{
    RecentFileNotOpenedDialog dialog(path, parent);

    return dialog.exec();
}

