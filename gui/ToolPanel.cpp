#include "ToolPanel.h"
#include "ToolPanelButton.h"

#include "tools/Tool.h"

#include "style/Style.h"

#include <QtCore/QEvent>

#include <QtWidgets/QVBoxLayout>
#include <QtGui/QPainter>
#include <QtGui/QLinearGradient>

ToolPanel::ToolPanel(QWidget *parent)
    : QWidget(parent)
{
    setMinimumWidth(64);
    setMaximumWidth(64);

    layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    layout->addStretch();
}

Tool*
ToolPanel::addTool(Tool *tool)
{
    ToolPanelButton *button = new ToolPanelButton(tool);

    layout->insertWidget(layout->count() - 1, button);
    buttons.append(button);

    button->installEventFilter(this);

    connect(button, SIGNAL(pressed()), SLOT(buttonPressed()));

    if(buttons.count() == 1)
    {
        current = button;
        current->setDown(true);

        emit toolChanged(tool, 0);
    }

    return tool;
}

QList<Tool*>
ToolPanel::tools() const
{
    QList<Tool*> list;
    foreach(const ToolPanelButton *button, buttons)
    {
        list.append(button->tool());
    }

    return list;
}

Tool*
ToolPanel::tool() const
{
    return current ? current->tool() : 0;
}

bool
ToolPanel::eventFilter(QObject *object, QEvent *event)
{
    ToolPanelButton *button = dynamic_cast<ToolPanelButton*>(object);
    if(!button)
    {
        return QWidget::eventFilter(object, event);
    }

    if(event->type() == QEvent::Show)
    {
        button->tool()->enableRelatedActions(true);

        if(!current)
        {
            current = button;
            button->setDown(true);

            emit toolChanged(button->tool(), 0);
        }
    }
    else if(event->type() == QEvent::Hide || event->type() == QEvent::HideToParent)
    {
        button->tool()->enableRelatedActions(false);

        if(current == button)
        {
            QVector<ToolPanelButton*> visible;
            foreach(ToolPanelButton *other, buttons)
            {
                if(other->isVisible())
                {
                    visible.append(other);
                }
            }

            int index = buttons.indexOf(button);
            if(index >= visible.count())
            {
                index = visible.count() - 1;
            }

            if(index >= 0 && index < visible.count())
            {
                current = visible[index];

                button->setDown(false);
                current->setDown(true);

                emit toolChanged(current->tool(), button->tool());
            }
            else
            {
                current = 0;

                button->setDown(false);

                emit toolChanged(0, button->tool());
            }
        }
    }

    return QWidget::eventFilter(object, event);
}

void
ToolPanel::buttonPressed()
{
    ToolPanelButton *button = static_cast<ToolPanelButton*>(sender());

    foreach(ToolPanelButton *other, buttons)
    {
        if(other != button)
        {
            other->setDown(false);
        }
    }

    ToolPanelButton *previous = current;
    current = button;

    emit toolChanged(current ? current->tool() : 0, previous ? previous->tool() : 0);
}

void
ToolPanel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QColor color = Style::baseColor();

    QLinearGradient g(rect().topLeft(), rect().topRight());
    g.setColorAt(0, color);
    g.setColorAt(1, color.darker(120));

    painter.fillRect(rect(), g);
}
