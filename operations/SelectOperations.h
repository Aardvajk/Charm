#ifndef SELECTOPERATIONS_H
#define SELECTOPERATIONS_H

#include <QtCore/QObject>

class Action;
class ActionList;
class Model;

class SelectOperations : public QObject
{
    Q_OBJECT

public:
    SelectOperations(ActionList *actions, Model *model, QObject *parent = 0);

private slots:
    void
    modelChanged();

    void
    selectAll();

    void
    selectNone();

    void
    selectInvert();

private:
    Model *model;

    Action *allAction;
    Action *noneAction;
    Action *invertAction;
};

#endif // SELECTOPERATIONS_H
