#ifndef ERRORDIALOG_H
#define ERRORDIALOG_H

#include <QtWidgets/QMessageBox>

class ErrorDialog : public QMessageBox
{
    Q_OBJECT

public:
    ErrorDialog(QWidget *parent = 0);

    static
    int
    show(QWidget *parent, const QString &text, const QString &informativeText);
};

#endif // ERRORDIALOG_H
