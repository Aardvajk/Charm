#include "FramePanel.h"

#include "debug/Assert.h"

#include "settings/Settings.h"

#include "style/Style.h"

#include "actions/Action.h"
#include "actions/ActionList.h"

#include "model/Model.h"

#include "gui/FrameSlider.h"
#include "gui/PanelLineEdit.h"
#include "gui/PanelButton.h"
#include "gui/PanelGroup.h"

#include "dialogs/AnimationEventDialog.h"

#include "common/StrictDoubleValidator.h"

#include "command/KeyFrameCommand.h"
#include "command/AnimationEventCommand.h"

#include <QtCore/QTimer>

#include <QtWidgets/QHBoxLayout>
#include <QtGui/QIntValidator>

FramePanel::FramePanel(Settings *settings, ActionList *actions, Model *model, QWidget *parent)
    : QWidget(parent),
      model(model),
      settings(settings),
      current(-1)
{
    Settings *child = settings->child("FramePanel");

    setAutoFillBackground(true);

    setMinimumHeight(48);
    setMaximumHeight(minimumHeight());

    QPalette pal = palette();
    pal.setColor(QPalette::Background, Style::baseColor().darker(140));

    setPalette(pal);

    toggleFrameAction = actions->add("Animation", "Toggle Keyframe");
    toggleFrameAction->setText("&Toggle Keyframe");
    toggleFrameAction->setShortcut(QKeySequence("F"));

    toggleFrameAction->setEnabled(false);

    connect(toggleFrameAction, SIGNAL(triggered()), SLOT(toggleFrame()));

    toggleEventAction = actions->add("Animation", "Toggle Event");
    toggleEventAction->setText("&Toggle Event");
    toggleEventAction->setShortcut(QKeySequence("E"));

    toggleEventAction->setEnabled(false);

    connect(toggleEventAction, SIGNAL(triggered()), SLOT(toggleEvent()));

    modifyEventAction = actions->add("Animation", "Modify Event");
    modifyEventAction->setText("&Modify Event");

    modifyEventAction->setEnabled(false);

    connect(modifyEventAction, SIGNAL(triggered()), SLOT(modifyEvent()));

    slider = new FrameSlider(this);
    slider->setEnabled(false);

    connect(slider, SIGNAL(valueChanged(float)), SLOT(sliderChanged(float)));
    connect(slider, SIGNAL(currentKeyPressed(int)), SLOT(sliderKeyChanged(int)));
    connect(slider, SIGNAL(currentKeyChanged(int)), SLOT(sliderKeyChanged(int)));
    connect(slider, SIGNAL(currentEventChanged(int)), SLOT(sliderEventChanged(int)));
    connect(slider, SIGNAL(currentEventDoubleClicked(int)), SLOT(modifyEvent()));

    edit = new PanelLineEdit(this);
    edit->setMaximumWidth(42);
    edit->setEnabled(false);

    edit->setValidator(new StrictDoubleValidator(1, 18, 2, edit));

    connect(edit, SIGNAL(textChanged(QString)), SLOT(editChanged(QString)));

    snapButton = makeButton("Snap", edit->sizeHint().height(), true, 42);
    connect(snapButton, SIGNAL(toggled(bool)), slider, SLOT(setSnapMode(bool)));

    viewButton = makeButton("View", edit->sizeHint().height(), false, 42);
    connect(viewButton, SIGNAL(toggled(bool)), SLOT(viewButtonToggled(bool)));

    playButton = makeButton("Play", edit->sizeHint().height(), true, 42);
    playButton->setCheckable(false);
    connect(playButton, SIGNAL(clicked()), SLOT(playButtonClicked()));

    PanelGroup *group = new PanelGroup(this);
    group->setMaximumHeight(edit->sizeHint().height() + 2);

    group->addWidget(edit);
    group->addWidget(snapButton);
    group->addWidget(viewButton);
    group->addWidget(playButton);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(1);

    layout->addWidget(slider, 2);
    layout->addWidget(group);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(timerTimeOut()));

    connect(model, SIGNAL(animationChanged(int)), SLOT(animationChanged(int)));
    connect(model, SIGNAL(changed()), SLOT(modelChanged()));
    connect(model, SIGNAL(modeChanged()), SLOT(modeChanged()));

    snapButton->setDown(child->child("snap")->value(true).toBool());
}

void
FramePanel::savingSettings()
{
    Settings *child = settings->child("FramePanel");

    child->clear();
    child->add("snap", snapButton->down());
}

void
FramePanel::sliderChanged(float value)
{
    QString text = QString::number(value, 'f', 2);

    edit->blockSignals(true);
    edit->setText(text);
    edit->blockSignals(false);

    updateModelFrame();
}

void
FramePanel::sliderKeyChanged(int index)
{
    if(index >= 0)
    {
        Assert(current >= 0 && current < model->animationCount());

        KeyFrameCommand *command = new KeyFrameCommand("Get Keyframe", model, this);
        command->get(current, slider->currentKey());

        model->endCommand(command);

        updateModelFrame();
    }
}

void
FramePanel::sliderEventChanged(int index)
{
    modifyEventAction->setEnabled(index >= 0);
}

void
FramePanel::editChanged(const QString &text)
{
    slider->blockSignals(true);
    slider->setValue(text.toFloat());
    slider->blockSignals(false);

    updateModelFrame();
}

void
FramePanel::currentAnimationChanged(int index)
{
    if(current != index)
    {
        current = index;

        if(index < 0)
        {
            slider->setEnabled(false);

            edit->setText("");
            edit->setEnabled(false);

            snapButton->setEnabled(false);
            viewButton->setEnabled(false);
            playButton->setEnabled(false);

            toggleFrameAction->setEnabled(false);
            toggleEventAction->setEnabled(false);
            modifyEventAction->setEnabled(false);

            updateModelFrame();
        }
        else
        {
            Assert(current >= 0 && current < model->animationCount());

            slider->setEnabled(true);
            slider->setKeyPositions(model->animation(current).keyFramePositions());
            slider->setEventPositions(model->animation(current).eventPositions());
            slider->setValue(1.0f);
            slider->setRange(1, model->animation(current).frames());

            edit->setText(QString::number(slider->value(), 'f', 2));
            edit->setEnabled(true);

            snapButton->setEnabled(true);
            viewButton->setEnabled(true);
            playButton->setEnabled(model->animation(current).keyFrameCount() > 0);

            toggleFrameAction->setEnabled(model->mode() == Mode::Animate);
            toggleEventAction->setEnabled(model->mode() == Mode::Animate);
            modifyEventAction->setEnabled(model->mode() == Mode::Animate && slider->currentEvent() >= 0);

            model->setUseKeyFrame(viewButton->down());

            updateModelFrame();
        }

        stopPlaying();
    }
}

void
FramePanel::animationChanged(int index)
{
    if(current == index)
    {
        Assert(current >= 0 && current < model->animationCount());

        slider->setKeyPositions(model->animation(current).keyFramePositions());
        slider->setEventPositions(model->animation(current).eventPositions());
        slider->setRange(1, model->animation(current).frames());

        playButton->setEnabled(model->animation(current).keyFrameCount() > 0);

        updateModelFrame();
        stopPlaying();
    }
}

void
FramePanel::toggleFrame()
{
    Assert(current >= 0 && current < model->animationCount());

    if(slider->currentKey() >= 0)
    {
        KeyFrameCommand *command = new KeyFrameCommand("Remove Keyframe", model, this);
        command->remove(current, slider->currentKey());

        model->endCommand(command);

        slider->setCurrentKey(-1);

        updateModelFrame();
    }
    else
    {
        KeyFrame key(slider->value(), model);

        KeyFrameCommand *command = new KeyFrameCommand("Add Keyframe", model, this);
        command->add(current, key);

        model->endCommand(command);

        QVector<float> v = model->animation(current).keyFramePositions();
        for(int i = 0; i < v.count(); ++i)
        {
            if(qFuzzyCompare(v[i], slider->value()))
            {
                slider->setCurrentKey(i);
            }
        }

        updateModelFrame();
    }
}

void
FramePanel::toggleEvent()
{
    Assert(current >= 0 && current < model->animationCount());

    if(slider->currentEvent() >= 0)
    {
        AnimationEventCommand *command = new AnimationEventCommand("Remove Event", model, this);
        command->remove(current, slider->currentEvent());

        model->endCommand(command);

        slider->setCurrentEvent(-1);
    }
    else
    {
        AnimationEvent event(slider->value());

        AnimationEventCommand *command = new AnimationEventCommand("Add Event", model, this);
        command->add(current, event);

        model->endCommand(command);

        QVector<float> v = model->animation(current).eventPositions();
        for(int i = 0; i < v.count(); ++i)
        {
            if(qFuzzyCompare(v[i], slider->value()))
            {
                slider->setCurrentEvent(i);
            }
        }
    }
}

void
FramePanel::modifyEvent()
{
    Assert(current >= 0 && current < model->animationCount());

    int index = slider->currentEvent();
    Assert(index >= 0 && index < model->animation(current).eventCount());

    AnimationEventDialog dialog(model->animation(current).event(index).details, this);

    if(dialog.exec() == QDialog::Accepted)
    {
        AnimationEvent event = model->animation(current).event(index);
        event.details = dialog.details();

        AnimationEventCommand *command = new AnimationEventCommand("Modify Animation", model, this);
        command->modify(current, index, event);

        model->endCommand(command);
    }
}

PanelButton*
FramePanel::makeButton(const QString &text, int height, bool down, int width) const
{
    PanelButton *button = new PanelButton(text);

    button->setMaximumHeight(height);
    button->setFrame(false);
    button->setMinimumHeight(height);
    button->setCheckable(true);
    button->setDown(down);
    button->setEnabled(false);

    button->setMaximumWidth(width);
    button->setMinimumWidth(width);

    return button;
}

void
FramePanel::viewButtonToggled(bool state)
{
    stopPlaying();

    model->setUseKeyFrame(state);
    updateModelFrame();
}

void
FramePanel::playButtonClicked()
{
    if(!timer->isActive())
    {
        Assert(current >= 0 && current < model->animationCount());

        model->setUseKeyFrame(true);

        timer->start();
        time.restart();

        slider->setBlockUserInteraction(true);

        edit->setEnabled(false);
        playButton->setText("Stop");
    }
    else
    {
        stopPlaying();
    }
}

void
FramePanel::modelChanged()
{
    viewButton->setDown(false);
    stopPlaying();
}

void
FramePanel::modeChanged()
{
    if(model->mode() == Mode::Animate)
    {
        toggleFrameAction->setEnabled(current >= 0);
        toggleEventAction->setEnabled(current >= 0);
        modifyEventAction->setEnabled(slider->currentEvent() >= 0);
    }
    else
    {
        toggleFrameAction->setEnabled(false);
        toggleEventAction->setEnabled(false);
        modifyEventAction->setEnabled(false);
    }
}

void
FramePanel::timerTimeOut()
{
    Assert(current >= 0 && current < model->animationCount());

    float delta = time.elapsed() / 1000.0f;
    float val = slider->value();

    float frames = model->animation(current).frames();
    float duration = model->animation(current).duration();

    float amount = delta / duration;

    val += frames * amount;

    slider->setValue(fmod(val, frames));

    time.restart();
}

void
FramePanel::updateModelFrame()
{
    if(current >= 0)
    {
        if(model->useKeyFrame())
        {
            Assert(current >= 0 && current < model->animationCount());

            if(model->animation(current).keyFrameCount() > 0)
            {
                model->setKeyFrame(model->animation(current).interpolatedKeyFrame(slider->value()));
                model->changePose();
            }
        }
        else
        {
            model->changePose();
        }
    }
    else
    {
        model->setUseKeyFrame(false);
        model->changePose();
    }
}

void
FramePanel::stopPlaying()
{
    if(timer->isActive())
    {
        timer->stop();
        model->setUseKeyFrame(viewButton->down());
        model->changePose();

        slider->setBlockUserInteraction(false);

        edit->setEnabled(true);
        playButton->setText("Play");
    }
}
