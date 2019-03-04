#ifndef PAINTPANEL_H
#define PAINTPANEL_H

#include <QtWidgets/QWidget>

class Action;
class ActionList;
class Model;
class PaintListBox;
class PanelButton;

class PaintPanel : public QWidget
{
    Q_OBJECT

public:
    PaintPanel(ActionList *actions, Model *model, QWidget *parent = 0);

private slots:
    void
    paletteChanged();

    void
    enableButtons();

    void
    paint();

    void
    add();

    void
    edit();

    void
    del();

private:
    QColor
    fromDialog(const QColor &initial);

    Model *model;
    PaintListBox *list;

    Action *paintAction;

    PanelButton *paintButton;
    PanelButton *addButton;
    PanelButton *editButton;
    PanelButton *delButton;
};

#endif // PAINTPANEL_H
