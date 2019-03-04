#include "Assert.h"

#include <QtCore/QDebug>
#include <QtWidgets/QMessageBox>

void
AssertImplementation(bool value, const QString &expr, const QString &file, const QString &method, int line)
{
    if(!value)
    {
        QString message = QString("Assertion failed:<br/><br/>%1<br/><br/>%2<br/>%3<br/>Line %4").arg(expr).arg(file).arg(method).arg(line);

        qDebug() << message;

        QMessageBox::critical(0, "Assertion Failed", message);
        throw AssertException();
    }
}
