#ifndef CLIPBOARDOPERATIONS_H
#define CLIPBOARDOPERATIONS_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>

class Action;
class ActionList;
class Model;

class ClipboardOperations : public QObject
{
    Q_OBJECT

public:
    ClipboardOperations(ActionList *actions, Model *model, QObject *parent = 0);

private slots:
    void
    modelChanged();

    void
    clipboardChanged();

    void
    cut();

    void
    copy();

    void
    paste();

private:
    QByteArray
    createMimeData();

    bool
    hasFormat() const;

    Model *model;

    Action *cutAction;
    Action *copyAction;
    Action *pasteAction;
};

#endif // CLIPBOARDOPERATIONS_H
