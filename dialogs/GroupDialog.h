#ifndef GROUPDIALOG_H
#define GROUPDIALOG_H

#include <QtWidgets/QDialog>

#include "model/Group.h"

class QLineEdit;

class GroupDialog : public QDialog
{
    Q_OBJECT

public:
    GroupDialog(const GroupDetails &details, QWidget *parent = 0);

    GroupDetails
    details() const { return det; }

private slots:
    void
    okClicked();

    void
    enableOk();

private:
    GroupDetails det;

    QLineEdit *nameEdit;

    QPushButton *okButton;
};

#endif // GROUPDIALOG_H
