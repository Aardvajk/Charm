#include "Command.h"

#include "model/Model.h"

Command::Command(Model *model, QObject *parent)
    : QObject(parent),
      data(0)
{
    model->beginCommand(this);
}

void
Command::begin(ModelData *modelData)
{
    data = modelData;
}
