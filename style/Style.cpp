#include "Style.h"

#include <QtWidgets/QStyleOption>
#include <QtWidgets/QStyleOptionMenuItem>
#include <QtGui/QPainter>
#include <QtGui/QLinearGradient>

namespace
{

void
menuGradient(QPainter *p, const QRect &rect)
{
    QColor color = Style::baseColor();

    QLinearGradient g(rect.topLeft(), rect.bottomLeft());
    g.setColorAt(0, color.darker(110));
    g.setColorAt(1, color.darker(140));

    p->fillRect(rect, g);
}

}

void
Style::drawControl(ControlElement element, const QStyleOption *opt, QPainter *p, const QWidget *w) const
{
    switch(element)
    {
        case QStyle::CE_MenuBarEmptyArea:
        {
            menuGradient(p, opt->rect.adjusted(0, 0, 0, -1));

            p->setPen(QColor(0, 0, 0, 200));
            p->drawLine(opt->rect.bottomLeft(), opt->rect.bottomRight());

            return;
        }

        case QStyle::CE_MenuBarItem:
        {
            const QStyleOptionMenuItem *mbo = static_cast<const QStyleOptionMenuItem*>(opt);

            uint alignment = Qt::AlignCenter | Qt::TextShowMnemonic | Qt::TextDontClip | Qt::TextSingleLine;

            if(!styleHint(SH_UnderlineShortcut, mbo, w))
            {
                alignment |= Qt::TextHideMnemonic;
            }

            menuGradient(p, opt->rect);

            if(mbo->state & State_Enabled)
            {
                if(mbo->state & State_Sunken)
                {
                    p->save();

                    p->setPen(QColor(64, 64, 64));
                    p->setBrush(QColor(0, 0, 0, 64));

                    p->drawRect(mbo->rect.adjusted(0, 1, -1, -1));

                    p->restore();
                }
                else if(mbo->state & State_Selected)
                {
                    p->fillRect(mbo->rect.adjusted(0, 1, 0, -1), QColor(0, 0, 0, 32));
                }
            }

            drawItemText(p, mbo->rect, alignment, mbo->palette, true, mbo->text);

            return;
        }

        default: break;
    }

    QProxyStyle::drawControl(element, opt, p, w);
}

QColor
Style::baseColor()
{
    return Qt::white;
}
