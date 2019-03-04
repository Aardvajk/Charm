#ifndef SETTINGSPANEL_H
#define SETTINGSPANEL_H

#include <QtWidgets/QWidget>

class Model;
class ActionList;
class AnchorWidget;
class AxisLockWidget;

class SettingsPanel : public QWidget
{
    Q_OBJECT

public:
    SettingsPanel(Model *model, ActionList *actions, QWidget *parent = 0);

    AnchorWidget*
    anchorWidget() const { return anchWidget; }

    AxisLockWidget*
    axisLockWidget() const { return axisWidget; }    

protected:
    virtual
    void
    paintEvent(QPaintEvent *event);

private:
    Model *model;

    AnchorWidget *anchWidget;
    AxisLockWidget *axisWidget;
};

#endif // SETTINGSPANEL_H
