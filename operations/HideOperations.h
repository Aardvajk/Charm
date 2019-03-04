#ifndef HIDEOPERATIONS_H
#define HIDEOPERATIONS_H

#include <QtCore/QObject>

class Action;
class ActionList;
class Model;

class HideOperations : public QObject
{
    Q_OBJECT

public:
    HideOperations(ActionList *actions, Model *model, QObject *parent = 0);

private slots:
    void
    modelChanged();

    void
    hide();

    void
    unhide();

    void
    unhideAll();

private:
    Model *model;

    Action *hideAction;
    Action *unhideAction;
    Action *unhideAllAction;
};

#endif // HIDEOPERATIONS_H
