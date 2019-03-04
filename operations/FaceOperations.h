#ifndef FACEOPERATIONS_H
#define FACEOPERATIONS_H

#include <QtCore/QObject>

class Action;
class ActionList;
class Model;

class FaceOperations : public QObject
{
    Q_OBJECT

public:
    FaceOperations(ActionList *actions, Model *model, QObject *parent = 0);

private slots:
    void
    modelChanged();

    void
    create();

    void
    reverse();

    void
    selectVerts();

    void
    groupSplit();

    void
    selectDegenerates();

    void
    clearPainting();

private:
    Model *model;

    Action *createAction;
    Action *reverseAction;
    Action *selectVertsAction;
    Action *groupSplitAction;
    Action *selectDegeneratesAction;
    Action *clearPaintingAction;
};

#endif // FACEOPERATIONS_H
