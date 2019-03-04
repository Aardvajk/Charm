#include "Fatal.h"

#include <QtCore/QCoreApplication>
#include <QtWidgets/QMessageBox>

void
Fatal(const QString &error)
{
    QMessageBox msg;

    msg.setText("The application has encountered the following fatal error and is about to close.");
    msg.setWindowTitle("Charm");
    msg.setIcon(QMessageBox::Critical);

    QString message;

    message = QString("<b>%1</b>").arg(error);

    message += "<br/><br/><hr/>If the problem persists, please contact <a href=\"mailto:support@aardvajk.com\">support@aardvajk.com</a> quoting the above error.";

    msg.setInformativeText(message);
    msg.exec();

    throw FatalException();
}
