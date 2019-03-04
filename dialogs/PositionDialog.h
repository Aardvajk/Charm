#ifndef POSITIONDIALOG_H
#define POSITIONDIALOG_H

#include <QtCore/QVector>

#include <QtWidgets/QDialog>

#include "maths/Maths.h"

class QPushButton;
class QLineEdit;
class QFormLayout;

class PositionDialog : public QDialog
{
    Q_OBJECT

public:
    PositionDialog(const Vec3 &pos, QWidget *parent = 0);

    Vec3
    value() const;

private slots:
    void
    okClicked();

    void
    enableOk();

private:
    void
    addRow(QFormLayout *layout, const QString &label, float pos);

    QPushButton *okButton;
    QVector<QLineEdit*> edits;

    Vec3 pos;
};

#endif // POSITIONDIALOG_H
