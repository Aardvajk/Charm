#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QtWidgets/QDialog>

#include "model/Group.h"

class QComboBox;
class QLineEdit;

class ExportDetails
{
public:
    ExportDetails();
    ExportDetails(int type, const QString &path, float scale);

    int type;
    QString path;
    float scale;
};

class ExportDialog : public QDialog
{
    Q_OBJECT

public:
    ExportDialog(const ExportDetails &details, QWidget *parent = 0);

    ExportDetails
    details() const { return det; }

private slots:
    void
    browseClicked();

    void
    okClicked();

private:
    QComboBox *typeCombo;
    QLineEdit *scaleEdit;
    QLineEdit *pathEdit;

    QPushButton *okButton;

    ExportDetails det;
};

#endif // EXPORTDIALOG_H
