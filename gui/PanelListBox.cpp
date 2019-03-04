#include "PanelListBox.h"

#include <QtWidgets/QScrollBar>
#include <QtGui/QMouseEvent>

PanelListBox::PanelListBox(QWidget *parent)
    : BasePanelListBox(parent),
      current(-1),
      drawSelection(false)
{
}

void
PanelListBox::clear()
{
    items.clear();

    int old = current;
    current = hover = -1;

    viewport()->update();

    if(old != current)
    {
        emit currentChanged(current);
    }
}

void
PanelListBox::setDrawSelection(bool state)
{
    if(drawSelection != state)
    {
        drawSelection = state;
        viewport()->update();
    }
}

void
PanelListBox::removeItem(int index)
{
    if(index >= 0 && index < items.count())
    {
        items.remove(index);

        hover = current = -1;
        emit currentChanged(-1);

        updateScroll();
        viewport()->update();
    }
}

QVariant
PanelListBox::currentData() const
{
    if(current >= 0 && current < items.count())
    {
        return items[current].data();
    }

    return QVariant();
}

void
PanelListBox::setCurrentItem(int index)
{
    if(current != index)
    {
        current = hover = index;

        ensureVisible(current);
        update();

        emit currentChanged(index);
    }
}

void
PanelListBox::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(current >= 0 && current < items.count())
    {
        emit itemDoubleClicked(current);
    }
}

void
PanelListBox::mouseMoveEvent(QMouseEvent *event)
{
    int item = (event->pos().y() + verticalScrollBar()->value()) / itemHeight();

    if(drawSelection && (item < 0 || item >= items.count()))
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
PanelListBox::mouseReleaseEvent(QMouseEvent *event)
{
    if(rect().contains(event->pos()))
    {
        if(hover >= 0 && hover < items.count())
        {
            if(current != hover)
            {
                current = hover;
                ensureVisible(current);

                viewport()->update();

                emit currentChanged(current);
            }

            emit clicked(hover);
        }
    }
}

bool
PanelListBox::event(QEvent *event)
{
    if(event->type() == QEvent::HoverLeave && drawSelection)
    {
        hover = -1;
        viewport()->update();
    }

    return BasePanelListBox::event(event);
}

bool
PanelListBox::drawItemSelected(int index) const
{
    return drawSelection && index == current;
}
