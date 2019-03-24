#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QtWidgets/QDialog>

#include "model/Group.h"

#include "maths/Vec3.h"

class QComboBox;
class QLineEdit;

class ExportDetails
{
public:
    ExportDetails();
    ExportDetails(int type, const QString &path, float scale, const Vec3 &offset);

    bool operator==(const ExportDetails &o) const;
    bool operator!=(const ExportDetails &o) const { return !(*this == o); }

    int type;
    QString path;
    float scale;
    Vec3 offset;
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
    QLineEdit *offsetEdit;
    QLineEdit *pathEdit;

    QPushButton *okButton;

    ExportDetails det;
};

#endif // EXPORTDIALOG_H
