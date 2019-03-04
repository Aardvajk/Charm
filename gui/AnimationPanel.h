#ifndef ANIMATIONPANEL_H
#define ANIMATIONPANEL_H

#include <QtWidgets/QWidget>

class Model;
class PanelListBox;
class PanelButton;

class AnimationPanel : public QWidget
{
    Q_OBJECT

public:
    AnimationPanel(Model *model, QWidget *parent = 0);

signals:
    void
    currentChanged(int index);

private slots:
    void
    animationsChanged();

    void
    animationChanged(int index);

    void
    currentIndexChanged(int index);

    void
    add();

    void
    duplicate();

    void
    edit();

    void
    del();

private:
    void
    enableButtons();

    Model *model;
    PanelListBox *list;

    PanelButton *dupButton;
    PanelButton *editButton;
    PanelButton *delButton;
};

#endif // ANIMATIONPANEL_H
