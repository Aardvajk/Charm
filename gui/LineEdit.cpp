#include "LineEdit.h"

#include "common/VariantAnimation.h"
#include "common/Color.h"

#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>

LineEdit::LineEdit(QWidget *parent)
    : QLineEdit(parent),
      deleteAlpha(0)
{
    setAttribute(Qt::WA_MouseTracking);

    QMargins margins = textMargins();
    margins.setRight(24);

    setTextMargins(margins);

    deleteButtonAnim = new VariantAnimation(this);
    deleteButtonAnim->setStartValue(0.0f);
    deleteButtonAnim->setDuration(200);

    connect(deleteButtonAnim, SIGNAL(valueChanged(QVariant)), SLOT(deleteAnimChanged(QVariant)));
    connect(this, SIGNAL(textChanged(QString)), SLOT(textChangedHandler(QString)));

    textChangedHandler(QString());
}

void
LineEdit::setHintText(const QString &text)
{
    hintText = text;
    update();
}

void
LineEdit::paintEvent(QPaintEvent *event)
{
    bool showHintText = false;
    QColor originalTextColor;

    blockSignals(true);

    if(isEnabled() && !hasFocus() && text().isEmpty() && !hintText.isEmpty())
    {
        setText(hintText);

        QPalette pal = palette();
        originalTextColor = pal.color(QPalette::Text);

        pal.setColor(QPalette::Text, Qt::darkGray);

        setPalette(pal);

        showHintText = true;
    }

    QLineEdit::paintEvent(event);

    if(showHintText)
    {
        setText(QString());

        QPalette pal = palette();
        pal.setColor(QPalette::Text, originalTextColor);

        setPalette(pal);
    }

    blockSignals(false);

    QPainter painter(this);

    QRect icon(0, 0, 16, 16);
    icon.moveCenter(iconRect().center());

    painter.drawPixmap(icon, QPixmap(":/resources/images/delete.png"));

    QColor color = setAlpha(palette().color(QPalette::Base), 255 - int(255.0f * deleteAlpha));

    painter.fillRect(icon, color);
}

void
LineEdit::mousePressEvent(QMouseEvent *event)
{
    mousePressOverDeleteIcon = iconRect().contains(event->pos());

    QLineEdit::mousePressEvent(event);
}

void
LineEdit::mouseReleaseEvent(QMouseEvent *event)
{
    if(mousePressOverDeleteIcon && !text().isEmpty())
    {
        deleteButtonAnim->setStartValue(deleteAlpha);
        deleteButtonAnim->setEndValue(0.0f);

        deleteButtonAnim->start();

        setText(QString());
    }

    QLineEdit::mouseReleaseEvent(event);
}

void
LineEdit::mouseMoveEvent(QMouseEvent *event)
{
    if(iconRect().contains(event->pos()))
    {
        setCursor(Qt::ArrowCursor);
        event->accept();
    }
    else
    {
        setCursor(Qt::IBeamCursor);
    }

    QLineEdit::mouseMoveEvent(event);
}

void
LineEdit::updateText(const QString &text)
{
    textChangedHandler(text);
}

void
LineEdit::deleteAnimChanged(const QVariant &value)
{
    deleteAlpha = value.toFloat();
    update();
}

void
LineEdit::textChangedHandler(const QString &text)
{
    if(!text.isEmpty())
    {
        deleteButtonAnim->setStartValue(deleteAlpha);
        deleteButtonAnim->setEndValue(1.0f);

        deleteButtonAnim->start();
    }
    else
    {
        deleteButtonAnim->setStartValue(deleteAlpha);
        deleteButtonAnim->setEndValue(0.0f);

        deleteButtonAnim->start();
    }
}

QRect
LineEdit::iconRect() const
{
    QRect iconRect = rect();
    iconRect.setLeft(rect().right() - 24);

    return iconRect;
}
