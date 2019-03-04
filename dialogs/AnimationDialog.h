#ifndef ANIMATIONDIALOG_H
#define ANIMATIONDIALOG_H

#include <QtWidgets/QDialog>

#include "model/Animation.h"

class QLineEdit;
class QSpinBox;

class AnimationDialog : public QDialog
{
    Q_OBJECT

public:
    enum Options { NoOptions = 0, TrackChanges = 1 };

    AnimationDialog(const AnimationDetails &details, Options options = NoOptions, QWidget *parent = 0);

    AnimationDetails
    details() const { return det; }

private slots:
    void
    okClicked();

    void
    enableOk();

private:
    AnimationDetails det;
    Options ops;

    QLineEdit *nameEdit;
    QSpinBox *framesSpin;
    QLineEdit *durationEdit;

    QPushButton *okButton;
};

#endif // ANIMATIONDIALOG_H
