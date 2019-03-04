#ifndef MODELVIEWTOOL_H
#define MODELVIEWTOOL_H

#include "tools/Tool.h"

class Model;
class ModelViewGrid;
class ModelView;

class QMouseEvent;
class QWheelEvent;

class ModelViewTool : public Tool
{
    Q_OBJECT

public:
    ModelViewTool(Settings *settings, Model *model, ModelViewGrid *viewGrid, QObject *parent = 0);

    virtual
    void
    connectSlots();

    virtual
    void
    disconnectSlots();

protected slots:
    virtual
    void
    mousePressed(ModelView *view, QMouseEvent *event) = 0;

    virtual
    void
    mouseMoved(ModelView *view, QMouseEvent *event) = 0;

    virtual
    void
    mouseReleased(ModelView *view, QMouseEvent *event) = 0;

    virtual
    void
    mouseWheelEvent(ModelView *view, QWheelEvent *event) = 0;

protected:
    Model*
    model() const { return mod; }

private:
    Model *mod;
    ModelViewGrid *viewGrid;
};

#endif // MODELVIEWTOOL_H
