#include "KeySequenceWidget.h"

#include <QtCore/QVector>
#include <QtCore/QMap>

#include <QtGui/QKeyEvent>

namespace
{

QKeySequence
keyEventToKeySequence(QKeyEvent *event)
{
    QVector<Qt::Key> ignoreKeys;
    ignoreKeys << Qt::Key_unknown << Qt::Key_Control << Qt::Key_Shift << Qt::Key_Alt << Qt::Key_Meta << Qt::Key_Menu;

    QMap<Qt::KeyboardModifier, Qt::Modifier> modifierMap;
    modifierMap[Qt::ShiftModifier] = Qt::SHIFT;
    modifierMap[Qt::ControlModifier] = Qt::CTRL;
    modifierMap[Qt::AltModifier] = Qt::ALT;
    modifierMap[Qt::MetaModifier] = Qt::META;

    int keyInt = event->key();
    Qt::Key key = static_cast<Qt::Key>(keyInt);

    if(ignoreKeys.contains(key))
    {
        return QKeySequence();
    }

    Qt::KeyboardModifiers modifiers = event->modifiers();

    foreach(Qt::KeyboardModifier mod, modifierMap.keys())
    {
        if(modifiers & mod)
        {
            keyInt += modifierMap[mod];
        }
    }

    return QKeySequence(keyInt);
}

QKeySequence
sequenceListToKeySequence(const QVector<QKeySequence> &seqs)
{
    QString text;

    for(int i = 0; i < seqs.count(); ++i)
    {
        text += seqs[i].toString(QKeySequence::NativeText);

        if(i < seqs.count() - 1)
        {
            text += ", ";
        }
    }

    return QKeySequence(text);
}

}

KeySequenceWidget::KeySequenceWidget(QWidget *parent)
    : LineEdit(parent),
      ignoreTextChanged(false)
{
    connect(this, SIGNAL(textChanged(QString)), SLOT(internalTextChanged(QString)));
}

QKeySequence
KeySequenceWidget::sequence() const
{
    return QKeySequence(sequenceListToKeySequence(seqs));
}

void
KeySequenceWidget::setSequence(const QKeySequence &sequence)
{
    seqs.clear();

    for(uint i = 0; i < sequence.count(); ++i)
    {
        seqs.append(QKeySequence(sequence[i]));
    }

    ignoreTextChanged = true;

    refreshText();

    ignoreTextChanged = false;

    emit sequenceChanged(QKeySequence(sequenceListToKeySequence(seqs)));
}

void
KeySequenceWidget::keyPressEvent(QKeyEvent *event)
{
    if(!isEnabled())
    {
        return;
    }

    QKeySequence keySequence = keyEventToKeySequence(event);

    if(!keySequence.isEmpty())
    {
        if(!seqs.contains(keySequence))
        {
            ignoreTextChanged = true;

            seqs.append(keySequence);
            refreshText();

            ignoreTextChanged = false;

            emit sequenceChanged(QKeySequence(sequenceListToKeySequence(seqs)));
        }
    }
}

void
KeySequenceWidget::internalTextChanged(const QString &text)
{
    if(ignoreTextChanged)
    {
        return;
    }

    ignoreTextChanged = true;

    seqs.clear();
    refreshText();

    ignoreTextChanged = false;

    emit sequenceChanged(QKeySequence(sequenceListToKeySequence(seqs)));
}

void
KeySequenceWidget::refreshText()
{
    setText(sequenceListToKeySequence(seqs).toString(QKeySequence::NativeText));
    updateText(text());
}
