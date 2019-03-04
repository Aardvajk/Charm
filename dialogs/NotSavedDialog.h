#ifndef NOTSAVEDDIALOG_H
#define NOTSAVEDDIALOG_H

#include <QtWidgets/QMessageBox>

class NotSavedDialog : public QMessageBox
{
    Q_OBJECT

public:
    NotSavedDialog(QWidget *parent = 0);

    static
    int
    show(QWidget *parent);
};

#endif // NOTSAVEDDIALOG_H
