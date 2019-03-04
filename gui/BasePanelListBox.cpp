#include "BasePanelListBox.h"

#include "style/Style.h"

#include "common/Color.h"

#include <QtWidgets/QScrollBar>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtGui/QLinearGradient>

BasePanelListBox::BasePanelListBox(QWidget *parent)
    : QAbstractScrollArea(parent),
      hover(-1)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    setAttribute(Qt::WA_MouseTracking);
    setAttribute(Qt::WA_Hover);

    QPalette pal = palette();
    pal.setColor(QPalette::Base, Style::baseColor().darker(105));

    setPalette(pal);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
}

void
BasePanelListBox::addItem(const QString &text, const QVariant &data)
{
    items.append(PanelListBoxItem(text, data));

    updateScroll();
    ensureVisible(items.count() - 1);
    viewport()->update();
}

void
BasePanelListBox::setItemText(int index, const QString &text)
{
    if(index >= 0 && index < items.count())
    {
        items[index].setText(text);
        viewport()->update();
    }
}

QFont
BasePanelListBox::itemFont(int index) const
{
    if(index >= 0 && index < items.count())
    {
        if(!items[index].fontValid())
        {
            return font();
        }

        return items[index].font();
    }

    return QFont();
}

void
BasePanelListBox::setItemFont(int index, const QFont &font)
{
    if(index >= 0 && index < items.count())
    {
        items[index].setFont(font);
        viewport()->update();
    }
}

void
BasePanelListBox::setItemColor(int index, const QColor &color)
{
    if(index >= 0 && index < items.count())
    {
        items[index].setColor(color);
        viewport()->update();
    }
}

void
BasePanelListBox::setItemData(int index, const QVariant &data)
{
    if(index >= 0 && index < items.count())
    {
        items[index].setData(data);
        viewport()->update();
    }
}

int
BasePanelListBox::heightForItems(int count) const
{
    return (itemHeight() * count) + 4;
}

void
BasePanelListBox::ensureVisible(int index)
{
    if(index < 0)
    {
        return;
    }

    int v = verticalScrollBar()->value();
    int y = itemHeight() * index;

    if(y - v < 0)
    {
        verticalScrollBar()->setValue(y);
    }

    if((y + itemHeight() + 2) - v > viewport()->rect().height())
    {
        verticalScrollBar()->setValue((y + 2) - (viewport()->rect().height() - itemHeight()));
    }
}

int
BasePanelListBox::currentScroll() const
{
    return verticalScrollBar()->value();
}

void
BasePanelListBox::setCurrentScroll(int value)
{
    verticalScrollBar()->setValue(value);
}

void
BasePanelListBox::paintEvent(QPaintEvent *event)
{
    QPainter painter(viewport());

    painter.fillRect(rect(), Style::baseColor().darker(105));

    for(int i = 0; i < items.count(); ++i)
    {
        int y = ((i * itemHeight()) - verticalScrollBar()->value()) + 1;

        int sw = (verticalScrollBar()->isVisible() ? verticalScrollBar()->width() : 0);

        QRect r(1, y, rect().width() - (4 + sw), itemHeight());

        painter.save();

        if(hover == i)
        {
            QColor color(175, 205, 245);

            QLinearGradient g(r.topLeft(), r.bottomLeft());
            g.setColorAt(0, setAlpha(color.lighter(120), 0.0f));
            g.setColorAt(1, setAlpha(color.lighter(120), 0.8f));

            painter.setPen(color);
            painter.setBrush(g);

            painter.drawRect(r.adjusted(0, 0, -1, -1));
        }

        if(drawItemSelected(i))
        {
            QColor color(145, 175, 215);

            QLinearGradient g(r.topLeft(), r.bottomLeft());
            g.setColorAt(0, setAlpha(color.lighter(120), 0.2f));
            g.setColorAt(1, setAlpha(color.lighter(120), 0.8f));

            painter.setPen(color);
            painter.setBrush(g);

            painter.drawRect(r.adjusted(0, 0, -1, -1));
        }

        painter.restore();

        paintItem(painter, r, i);
    }
}

void
BasePanelListBox::paintItem(QPainter &painter, const QRect &rect, int index)
{
    QColor c = items[index].color();
    if(!c.isValid())
    {
        c = Qt::black;
    }

    painter.setPen(c);

    QFont oldFont = painter.font();
    if(items[index].fontValid())
    {
        painter.setFont(items[index].font());
    }

    painter.drawText(rect.adjusted(5, 0, 0, 0), Qt::AlignLeft | Qt::AlignVCenter, items[index].text());
    painter.setFont(oldFont);
}

void
BasePanelListBox::resizeEvent(QResizeEvent *event)
{
    updateScroll();
}

void
BasePanelListBox::updateScroll()
{
    verticalScrollBar()->setRange(0, (itemHeight() * items.count()) - rect().height() + 4);
    verticalScrollBar()->setPageStep(rect().height());
    verticalScrollBar()->setSingleStep(itemHeight());
}

int
BasePanelListBox::itemHeight() const
{
    QFontMetrics fm(font());

    return fm.height() + 6;
}
