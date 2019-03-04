#ifndef WEIGHTPANEL_H
#define WEIGHTPANEL_H

#include <QtWidgets/QWidget>

class Model;
class WeightCombo;

class WeightPanel : public QWidget
{
    Q_OBJECT

public:
    WeightPanel(Model *model, QWidget *parent = 0);

private slots:
    void
    modelChanged();

    void
    comboChanged();

private:
    Model *model;
    QVector<WeightCombo*> combos;

    bool ignoreChange;
};

#endif // WEIGHTPANEL_H
