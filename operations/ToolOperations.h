#ifndef TOOLOPERATIONS_H
#define TOOLOPERATIONS_H

#include <QtCore/QObject>

class ActionList;
class Model;

class ToolOperations : public QObject
{
    Q_OBJECT

public:
    ToolOperations(ActionList *actions, Model *model, QObject *parent = 0);

private slots:
    void
    selectUnclosed();

private:
    Model *model;
};

#endif // TOOLOPERATIONS_H
