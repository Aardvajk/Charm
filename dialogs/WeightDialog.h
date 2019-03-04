#ifndef WEIGHTDIALOG_H
#define WEIGHTDIALOG_H

#include <QtWidgets/QDialog>

#include "model/WeightInfo.h"

class Model;
class QBoxLayout;
class QComboBox;
class QLineEdit;

class WeightDialog : public QDialog
{
    Q_OBJECT

public:
    WeightDialog(Model *model, const WeightInfo &info, QWidget *parent = 0);

    WeightInfo
    weights() const;

private slots:
    void
    okClicked();

    void
    enableOk();

    void
    comboChanged();

private:
    void
    addRow(QBoxLayout *layout, int row);

    void
    populateCombo(QComboBox *combo, int row);

    WeightInfo
    makeCurrentInfo() const;

    Model *model;

    WeightInfo info;

    QPushButton *okButton;

    QVector<QComboBox*> combos;
    QVector<QLineEdit*> edits;
};

#endif // WEIGHTDIALOG_H
