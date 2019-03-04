#include "RadioBox.h"

#include "gui/PanelButton.h"

#include <QtWidgets/QLayout>

RadioBox::RadioBox(QWidget *parent, Qt::Orientation orientation)
    : PanelGroup(parent, orientation),
      buttonMinSize(0, 22),
      buttonMaxSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX)
{
}

void
RadioBox::setButtonMinimumSize(const QSize &size)
{
    buttonMinSize = size;

    foreach(PanelButton *button, buttons)
    {
        button->setMinimumSize(buttonMinSize);
    }
}

void
RadioBox::setButtonMaximumSize(const QSize &size)
{
    buttonMaxSize = size;

    if(buttonMaxSize.width() <= 0)
    {
        buttonMaxSize.setWidth(QWIDGETSIZE_MAX);
    }

    if(buttonMaxSize.height() <= 0)
    {
        buttonMaxSize.setHeight(QWIDGETSIZE_MAX);
    }

    foreach(PanelButton *button, buttons)
    {
        button->setMaximumSize(buttonMaxSize);
    }
}

void
RadioBox::addItem(const QString &text, const QVariant &data)
{
    PanelButton *button = new PanelButton(text);
    button->setData(data);
    button->setFrame(false);
    button->setCheckable(true);

    button->setMinimumSize(buttonMinSize);
    button->setMaximumSize(buttonMaxSize);

    if(!buttons.isEmpty())
    {
        button->setDown(false);
    }

    buttons.append(button);

    connect(button, SIGNAL(toggled(bool)), SLOT(buttonToggled(bool)));

    addWidget(button);
}

QVariant
RadioBox::value() const
{
    foreach(PanelButton *button, buttons)
    {
        if(button->down())
        {
            return button->data();
        }
    }

    return QVariant();
}

void
RadioBox::setCurrentData(const QVariant &data)
{
    foreach(PanelButton *button, buttons)
    {
        if(button->data() == data)
        {
            button->setDown(true);
            break;
        }
    }
}

void
RadioBox::buttonToggled(bool state)
{
    PanelButton *button = static_cast<PanelButton*>(sender());

    if(!button->down())
    {
        button->setDown(true);
    }
    else
    {
        foreach(PanelButton *other, buttons)
        {
            if(other != button)
            {
                other->blockSignals(true);
                other->setDown(false);
                other->blockSignals(false);
            }
        }

        emit valueChanged(button->data());
    }
}

