#include "PanelLineEdit.h"

#include "style/Style.h"

#include <QtWidgets/QApplication>
#include <QtGui/QKeyEvent>
#include <QtGui/QPainter>

PanelLineEdit::PanelLineEdit(QWidget *parent)
    : QLineEdit(parent),
      frame(false)
{
    QLineEdit::setFrame(false);

    QPalette pal = palette();

    baseColor = pal.color(QPalette::Active, QPalette::Base);
    disabledColor = pal.color(QPalette::Disabled, QPalette::Base);

    pal.setColor(QPalette::Active, QPalette::Base, pal.color(QPalette::Disabled, QPalette::Base));
    pal.setColor(QPalette::Inactive, QPalette::Base, pal.color(QPalette::Disabled, QPalette::Base));
    pal.setColor(QPalette::Disabled, QPalette::Base, disabledColor.darker(110));

    QMargins margins = textMargins();
    margins.setLeft(2);

    setTextMargins(margins);

    setPalette(pal);
}

void
PanelLineEdit::setFrame(bool state)
{
    frame = state;

    QMargins margins = textMargins();
    margins.setLeft(frame ? 3 : 2);
    margins.setBottom(frame ? 1 : 0);

    setTextMargins(margins);

    update();
}

void
PanelLineEdit::paintEvent(QPaintEvent *event)
{
    QLineEdit::paintEvent(event);

    if(frame)
    {
        QPainter painter(this);
        painter.setPen(Style::baseColor().darker(240));
        painter.setBrush(Qt::NoBrush);

        painter.drawRect(rect().adjusted(0, 0, -1, -1));
    }
}

void
PanelLineEdit::focusInEvent(QFocusEvent *event)
{
    if(isEnabled() && !isReadOnly())
    {
        qApp->installEventFilter(this);

        QPalette pal = palette();
        pal.setColor(QPalette::Active, QPalette::Base, baseColor);

        setPalette(pal);

        QLineEdit::focusInEvent(event);
    }
}

void
PanelLineEdit::focusOutEvent(QFocusEvent *event)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Active, QPalette::Base, disabledColor);

    setPalette(pal);

    QLineEdit::focusOutEvent(event);
}

void
PanelLineEdit::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Tab || event->key() == Qt::Key_Return || event->key() == Qt::Key_Escape)
    {
        loseFocus();
        return;
    }

    QLineEdit::keyPressEvent(event);
}

bool
PanelLineEdit::eventFilter(QObject *object, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress && object != this)
    {
        loseFocus();
    }

    if(event->type() == QEvent::NonClientAreaMouseButtonPress || event->type() == QEvent::ApplicationDeactivate)
    {
        loseFocus();
    }

    if(event->type() == QEvent::FocusIn && object != this)
    {
        QWidget *widget = dynamic_cast<QWidget*>(object);

        if(widget && widget->focusPolicy() != Qt::NoFocus)
        {
            loseFocus();
            widget->setFocus();
        }
    }

    return QLineEdit::eventFilter(object, event);
}

void
PanelLineEdit::loseFocus()
{
    qApp->removeEventFilter(this);

    if(parentWidget())
    {
        parentWidget()->setFocus();
    }
}
