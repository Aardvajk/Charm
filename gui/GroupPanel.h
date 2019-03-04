#ifndef GROUPPANEL_H
#define GROUPPANEL_H

#include <QtWidgets/QWidget>

class Model;
class PanelListBox;
class PanelButton;

class GroupPanel : public QWidget
{
    Q_OBJECT

public:
    GroupPanel(Model *model, QWidget *parent = 0);

private slots:
    void
    groupsChanged();

    void
    select();

    void
    addNew();

    void
    add();

    void
    edit();

    void
    del();

    void
    enableButtons();

private:
    Model *model;
    PanelListBox *list;

    PanelButton *selectButton;
    PanelButton *newButton;
    PanelButton *addButton;
    PanelButton *editButton;
    PanelButton *delButton;
};

#endif // GROUPPANEL_H
