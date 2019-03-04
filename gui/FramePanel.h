#ifndef FRAMEPANEL_H
#define FRAMEPANEL_H

#include <QtCore/QTime>

#include <QtWidgets/QWidget>

class Action;
class ActionList;
class Model;
class FrameSlider;
class PanelLineEdit;
class PanelButton;
class QTimer;
class Settings;

class FramePanel : public QWidget
{
    Q_OBJECT

public:
    FramePanel(Settings *settings, ActionList *actions, Model *model, QWidget *parent = 0);

public slots:
    void
    savingSettings();

private slots:
    void
    sliderChanged(float value);

    void
    sliderKeyChanged(int index);

    void
    sliderEventChanged(int index);

    void
    editChanged(const QString &text);

    void
    currentAnimationChanged(int index);

    void
    animationChanged(int index);

    void
    toggleFrame();

    void
    toggleEvent();

    void
    modifyEvent();

    void
    viewButtonToggled(bool state);

    void
    playButtonClicked();

    void
    modelChanged();

    void
    modeChanged();

    void
    timerTimeOut();

private:
    PanelButton*
    makeButton(const QString &text, int height, bool down, int width) const;

    void
    updateModelFrame();

    void
    stopPlaying();

    Model *model;
    Settings *settings;

    int current;

    FrameSlider *slider;

    PanelLineEdit *edit;
    PanelButton *snapButton;
    PanelButton *viewButton;
    PanelButton *playButton;

    Action *toggleFrameAction;
    Action *toggleEventAction;
    Action *modifyEventAction;

    QTimer *timer;
    QTime time;
};

#endif // FRAMEPANEL_H
