#ifndef ANIMATIONOPERATIONS_H
#define ANIMATIONOPERATIONS_H

#include <QtCore/QObject>

class Action;
class ActionList;
class Model;

class AnimationOperations : public QObject
{
    Q_OBJECT

public:
    AnimationOperations(ActionList *actions, Model *model, QObject *parent = 0);

private slots:
    void
    modeChanged();

    void
    reset();

    void
    swapSelected();

    void
    swapAll();

private:
    void
    swap(bool onlySelected);

    Model *model;

    Action *resetAction;
    Action *swapSelectedAction;
    Action *swapAllAction;
};

#endif // ANIMATIONOPERATIONS_H
