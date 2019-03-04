#ifndef CURSOROPERATIONS_H
#define CURSOROPERATIONS_H

#include <QtCore/QObject>

class Action;
class ActionList;
class Model;

class CursorOperations : public QObject
{
    Q_OBJECT

public:
    CursorOperations(ActionList *actions, Model *model, QObject *parent = 0);

private slots:
    void
    selectionCentre();

    void
    origin();

    void
    originToCursor();

private:
    Model *model;

    Action *selectionCentreAction;
    Action *originAction;
    Action *originToCursorAction;
};

#endif // CURSOROPERATIONS_H
