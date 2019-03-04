#ifndef JOINTDIALOG_H
#define JOINTDIALOG_H

#include <QtWidgets/QDialog>

#include "model/Joint.h"

class Model;
class QLineEdit;
class QComboBox;

class JointDialog : public QDialog
{
    Q_OBJECT

public:
    JointDialog(Model *model, int joint, QWidget *parent = 0);

    JointDetails
    details() const { return det; }

private slots:
    void
    okClicked();

    void
    enableOk();

private:
    JointDetails det;

    QLineEdit *nameEdit;
    QComboBox *parentCombo;
    QComboBox *pairedCombo;

    QPushButton *okButton;
};

#endif // JOINTDIALOG_H
