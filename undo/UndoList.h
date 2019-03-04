#ifndef UNDOLIST_H
#define UNDOLIST_H

#include <QtCore/QObject>
#include <QtCore/QVector>

class Command;

class UndoList : public QObject
{
    Q_OBJECT

public:
    UndoList(QObject *parent = 0);

    void
    addCommand(Command *command);

    void
    clear();

    bool
    canUndo() const;

    bool
    canRedo() const;

    bool
    isModified() const;

    QString
    lastCommandName() const;

    QString
    nextCommandName() const;

signals:
    void
    undoStateChanged();

    void
    modifiedStateChanged(bool state);

    void
    undoTriggered();

public slots:
    void
    undo();

    void
    redo();

    void
    setSavePoint();

private:
    QVector<Command*> data;
    int pos;
    int save;
};

#endif // UNDOLIST_H
