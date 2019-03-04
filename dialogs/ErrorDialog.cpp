#include "ErrorDialog.h"

#include <QtCore/QCoreApplication>
#include <QtWidgets/QStyle>

ErrorDialog::ErrorDialog(QWidget *parent)
    : QMessageBox(parent)
{
    setWindowTitle(QCoreApplication::applicationName());
    setIcon(QMessageBox::Critical);
}

int
ErrorDialog::show(QWidget *parent, const QString &text, const QString &informativeText)
{
    ErrorDialog dialog(parent);
    dialog.setText(text);
    dialog.setInformativeText(informativeText + "<br/><hr/>If the problem persists, please contat <a href = \"mailto:aardvajk@live.co.uk\">aardvajk@live.co.uk</a> for further assistance.");

    return dialog.exec();
}

