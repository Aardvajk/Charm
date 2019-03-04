#ifndef VERTEXOPERATIONS_H
#define VERTEXOPERATIONS_H

#include <QtCore/QObject>

class Action;
class ActionList;
class Model;

class VertexOperations : public QObject
{
    Q_OBJECT

public:
    VertexOperations(ActionList *actions, Model *model, QWidget *parent = 0);

private slots:
    void
    modelChanged();

    void
    axisLocked(int axis, bool state);

    void
    snap();

    void
    weld();

    void
    split();

    void
    splitEdge();

    void
    editWeights();

    void
    clearWeights();

    void
    selectUnassigned();

    void
    selectUnclosed();

    void
    positionEdit();

    void
    snapToGrid();

    void
    fixNormals();

    void
    unfixNormals();

private:
    Model *model;

    QWidget *parentWidget;

    Action *snapAction;
    Action *weldAction;
    Action *splitAction;
    Action *splitEdgeAction;

    Action *editWeightsAction;
    Action *clearWeightsAction;
    Action *selectUnassignedAction;

    Action *selectUnclosedAction;

    Action *positionEditAction;
    Action *snapToGridAction;

    Action *fixNormalsAction;
    Action *unfixNormalsAction;

    bool axes[3];
};

#endif // VERTEXOPERATIONS_H
