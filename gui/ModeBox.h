#ifndef MODEBOX_H
#define MODEBOX_H

#include "gui/RadioBox.h"

class Model;

class ModeBox : public RadioBox
{
    Q_OBJECT

public:
    ModeBox(Model *model, QWidget *parent = 0);

private slots:
    void
    changed(const QVariant &data);

    void
    modelChanged();

private:
    Model *model;
};

#endif // MODEBOX_H
