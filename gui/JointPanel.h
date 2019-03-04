#ifndef JOINTPANEL_H
#define JOINTPANEL_H

#include <QtWidgets/QWidget>

class Model;
class PanelMultiListBox;
class PanelButton;

class JointPanel : public QWidget
{
    Q_OBJECT

public:
    JointPanel(Model *model, QWidget *parent = 0);

private slots:
    void
    modelChanged();

    void
    currentChanged(const QVector<int> &items);

    void
    weightClicked();

    void
    upClicked();

    void
    downClicked();

    void
    editClicked();

private:
    void
    enableButtons();

    void
    changeOrder(int direction);

    Model *model;

    PanelMultiListBox *list;
    PanelButton *weightButton;
    PanelButton *upButton;
    PanelButton *downButton;
    PanelButton *editButton;

    bool ignoreModelChange;
};

#endif // JOINTPANEL_H
