#ifndef DELETEOPERATIONS_H
#define DELETEOPERATIONS_H

#include <QtCore/QObject>

class Action;
class ActionList;
class Model;

class DeleteOperations : public QObject
{
    Q_OBJECT

public:
    DeleteOperations(ActionList *actions, Model *model, QObject *parent = 0);

private slots:
    void
    modelChanged();

    void
    del();

private:
    Model *model;

    Action *delAction;
};

#endif // DELETEOPERATIONS_H
