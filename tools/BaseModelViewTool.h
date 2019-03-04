#ifndef BASEMODELVIEWTOOL_H
#define BASEMODELVIEWTOOL_H

#include "ModelViewTool.h"

#include "maths/Vec3.h"

#include <QtCore/QPointF>

class BaseModelViewTool : public ModelViewTool
{
    Q_OBJECT

public:
    BaseModelViewTool(Settings *settings, Model *model, ModelViewGrid *viewGrid, QObject *parent = 0);

protected slots:
    virtual
    void
    mousePressed(ModelView *view, QMouseEvent *event);

    virtual
    void
    mouseMoved(ModelView *view, QMouseEvent *event);

    virtual
    void
    mouseReleased(ModelView *view, QMouseEvent *event);

    virtual
    void
    mouseWheelEvent(ModelView *view, QWheelEvent *event);

private:
    enum BaseState { NoBaseState, BaseStateRotating, BaseStateDragging };

    BaseState baseState;

    QPointF rotationPos;
    QPointF dragPos;
};

#endif // BASEMODELVIEWTOOL_H
