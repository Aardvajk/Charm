#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QtWidgets/QDialog>

class ActionList;

class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    OptionsDialog(ActionList *actions, QWidget *parent = 0);

signals:
    void
    commit();

public slots:
    virtual
    void
    accept();

private slots:
    void
    apply();
};

#endif // OPTIONSDIALOG_H
