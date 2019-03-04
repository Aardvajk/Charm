#ifndef MODIFYOPERATIONS_H
#define MODIFYOPERATIONS_H

#include <QtCore/QObject>

class Action;
class ActionList;
class Model;

class ModifyOperations : public QObject
{
    Q_OBJECT

public:
    ModifyOperations(ActionList *actions, Model *model, QObject *parent = 0);

private slots:
    void
    modelChanged();

    void
    flatten();

    void
    flip();

    void
    mirror();

    void
    zero();

private:
    Model *model;

    Action *flattenAction[3];
    Action *flipAction[3];
    Action *mirrorAction[3];
    Action *zeroAction[3];
};

#endif // MODIFYOPERATIONS_H
