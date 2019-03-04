#ifndef KEYBOARDOPTIONS_H
#define KEYBOARDOPTIONS_H

#include <QtWidgets/QWidget>

class ActionList;
class LineEdit;
class QTreeWidget;
class QTreeWidgetItem;
class KeySequenceWidget;

class KeyboardOptions : public QWidget
{
    Q_OBJECT

public:
    KeyboardOptions(ActionList *actions, QWidget *parent = 0);

public slots:
    void
    commit();

private slots:
    void
    currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void
    keySequenceChanged(const QKeySequence &sequence);

    void
    filterChanged(const QString &text);

private:
    void
    populateTree();

    ActionList *actions;

    QTreeWidget *tree;

    LineEdit *filterEdit;
    KeySequenceWidget *keyWidget;
    QWidget *keyLabel;
};

#endif // KEYBOARDOPTIONS_H
