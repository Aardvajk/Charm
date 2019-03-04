#include "ModelViewTool.h"

#include "views/ModelViewGrid.h"

ModelViewTool::ModelViewTool(Settings *settings, Model *model, ModelViewGrid *viewGrid, QObject *parent)
    : Tool(settings, parent),
      mod(model),
      viewGrid(viewGrid)
{
}

void
ModelViewTool::connectSlots()
{
    connect(viewGrid, SIGNAL(mousePressed(ModelView*,QMouseEvent*)), this, SLOT(mousePressed(ModelView*,QMouseEvent*)));
    connect(viewGrid, SIGNAL(mouseMoved(ModelView*,QMouseEvent*)), this, SLOT(mouseMoved(ModelView*,QMouseEvent*)));
    connect(viewGrid, SIGNAL(mouseReleased(ModelView*,QMouseEvent*)), this, SLOT(mouseReleased(ModelView*,QMouseEvent*)));
    connect(viewGrid, SIGNAL(mouseWheelEvent(ModelView*,QWheelEvent*)), this, SLOT(mouseWheelEvent(ModelView*,QWheelEvent*)));
}

void
ModelViewTool::disconnectSlots()
{
    disconnect(viewGrid, SIGNAL(mousePressed(ModelView*,QMouseEvent*)), this, SLOT(mousePressed(ModelView*,QMouseEvent*)));
    disconnect(viewGrid, SIGNAL(mouseMoved(ModelView*,QMouseEvent*)), this, SLOT(mouseMoved(ModelView*,QMouseEvent*)));
    disconnect(viewGrid, SIGNAL(mouseReleased(ModelView*,QMouseEvent*)), this, SLOT(mouseReleased(ModelView*,QMouseEvent*)));
    disconnect(viewGrid, SIGNAL(mouseWheelEvent(ModelView*,QWheelEvent*)), this, SLOT(mouseWheelEvent(ModelView*,QWheelEvent*)));
}
