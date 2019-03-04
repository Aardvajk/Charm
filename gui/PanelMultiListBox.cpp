#include "PanelMultiListBox.h"

#include <QtGui/QMouseEvent>
#include <QtWidgets/QScrollBar>

PanelMultiListBox::PanelMultiListBox(QWidget *parent)
    : BasePanelListBox(parent)
{
}

void
PanelMultiListBox::clear()
{
    bool any = !current.isEmpty();

    items.clear();
    current.clear();

    viewport()->update();

    if(any)
    {
        emit currentChanged(current);
    }
}

void
PanelMultiListBox::setCurrentItems(const QVector<int> &items)
{
    bool any = current != items;

    current = items;
    viewport()->update();

    if(any)
    {
        emit currentChanged(current);
    }
}

void
PanelMultiListBox::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(current.count() == 1)
    {
        emit itemDoubleClicked(current.front());
    }
}

void
PanelMultiListBox::mouseMoveEvent(QMouseEvent *event)
{
    int item = (event->pos().y() + verticalScrollBar()->value()) / itemHeight();

    if(item < 0 || item >= items.count())
    {
        hover = -1;
        viewport()->update();
    }

    if(item != hover && item >= 0 && item < items.count())
    {
        hover = item;
        viewport()->update();
    }

    BasePanelListBox::mouseMoveEvent(event);
}

void
PanelMultiListBox::mouseReleaseEvent(QMouseEvent *event)
{
    if(rect().contains(event->pos()))
    {
        if(hover >= 0 && hover < items.count())
        {
            if(!current.contains(hover))
            {
                if(!(event->modifiers() & Qt::ControlModifier))
                {
                    current.clear();
                }

                current.append(hover);
                viewport()->update();

                emit currentChanged(current);
            }
            else if(event->modifiers() & Qt::ControlModifier)
            {
                current.remove(current.indexOf(hover));
                viewport()->update();

                emit currentChanged(current);
            }
            else
            {
                current.clear();

                current.append(hover);
                viewport()->update();

                emit currentChanged(current);
            }
        }
        else if(!(event->modifiers() & Qt::ControlModifier))
        {
            bool any = !current.isEmpty();

            current.clear();
            viewport()->update();

            if(any)
            {
                emit currentChanged(current);
            }
        }
    }
}

bool
PanelMultiListBox::event(QEvent *event)
{
    if(event->type() == QEvent::HoverLeave)
    {
        hover = -1;
        viewport()->update();
    }

    return BasePanelListBox::event(event);
}

bool
PanelMultiListBox::drawItemSelected(int index) const
{
    return current.contains(index);
}
