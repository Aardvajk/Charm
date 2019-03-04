#ifndef CONVERTOPERATIONS_H
#define CONVERTOPERATIONS_H

#include <QtCore/QObject>

#include "dialogs/ExportDialog.h"

class Action;
class ActionList;
class Model;
class Graphics;

class ConvertOperations : public QObject
{
    Q_OBJECT

public:
    ConvertOperations(ActionList *actions, Model *model, Graphics *graphics, QWidget *parentWidget, QObject *parent = 0);

private slots:
    void
    input();

    void
    output();

private:
    bool
    outputModel(const ExportDetails &details);

    Model *model;
    Graphics *graphics;
    QWidget *parentWidget;
};

#endif // IMPORTOPERATIONS_H
