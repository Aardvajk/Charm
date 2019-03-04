#ifndef KEYSEQUENCEWIDGET_H
#define KEYSEQUENCEWIDGET_H

#include "gui/LineEdit.h"

#include <QtCore/QVector>
#include <QtGui/QKeySequence>

class KeySequenceWidget : public LineEdit
{
    Q_OBJECT

public:
    KeySequenceWidget(QWidget *parent = 0);

    QKeySequence
    sequence() const;

public slots:
    void
    setSequence(const QKeySequence &sequence);

signals:
    void
    sequenceChanged(const QKeySequence &sequence);

protected:
    virtual
    void
    keyPressEvent(QKeyEvent *event);

private slots:
    void
    internalTextChanged(const QString &text);

private:
    void
    refreshText();

    QVector<QKeySequence> seqs;
    bool ignoreTextChanged;
};

#endif // KEYSEQUENCEWIDGET_H
