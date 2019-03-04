#ifndef ANIMATIONEVENTDIALOG_H
#define ANIMATIONEVENTDIALOG_H

#include <QtWidgets/QDialog>

#include "model/AnimationEvent.h"

class QLineEdit;
class QPlainTextEdit;

class AnimationEventDialog : public QDialog
{
    Q_OBJECT

public:
    AnimationEventDialog(const AnimationEventDetails &details, QWidget *parent = 0);

    AnimationEventDetails
    details() const { return det; }

private slots:
    void
    okClicked();

    void
    enableOk();

private:
    AnimationEventDetails det;

    QLineEdit *frameEdit;
    QPlainTextEdit *dataEdit;

    QPushButton *okButton;
};

#endif // ANIMATIONEVENTDIALOG_H
