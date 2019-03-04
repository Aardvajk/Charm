#include "BaseModelViewTool.h"

#include "maths/Maths.h"

#include "views/ModelView.h"

#include <QtGui/QMouseEvent>
#include <QtGui/QWheelEvent>

BaseModelViewTool::BaseModelViewTool(Settings *settings, Model *model, ModelViewGrid *viewGrid, QObject *parent)
    : ModelViewTool(settings, model, viewGrid, parent),
      baseState(NoBaseState)
{
}

void
BaseModelViewTool::mousePressed(ModelView *view, QMouseEvent *event)
{
    if(event->button() == Qt::MiddleButton)
    {
        baseState = BaseStateRotating;
        rotationPos = QPointF(float(event->pos().x()), float(event->pos().y()));
    }
    else if(event->button() == Qt::LeftButton && event->modifiers() & Qt::ControlModifier)
    {
        baseState = BaseStateDragging;
        dragPos = view->screenToZPlane(event->pos());
    }
}

void
BaseModelViewTool::mouseMoved(ModelView *view, QMouseEvent *event)
{
    if(baseState == BaseStateRotating)
    {
        Vec3 n(float(event->pos().x()), float(event->pos().y()), 0);
        Vec3 d = n - Vec3(rotationPos.x(), rotationPos.y(), 0);

        float vx = float(view->width()) * 0.5f;
        float vy = float(view->height()) * 0.5f;

        float yaw = (d.x / vx) * pi();
        float pitch = (d.y / vy) * pi();

        view->setRotationVector(view->rotationVector() - Vec3(pitch, yaw, 0));

        rotationPos = QPointF(n.x, n.y);
    }
    else if(baseState == BaseStateDragging)
    {
        QPointF n = view->screenToZPlane(event->pos());
        QPointF d = n - dragPos;

        view->setPositionImmediate(view->position() - Vec3(d.x(), d.y(), 0));

        dragPos = view->screenToZPlane(event->pos());
    }
}

void
BaseModelViewTool::mouseReleased(ModelView *view, QMouseEvent *event)
{
    baseState = NoBaseState;
}

void
BaseModelViewTool::mouseWheelEvent(ModelView *view, QWheelEvent *event)
{
    if(event->buttons() == Qt::NoButton)
    {
        float mul = 1.0f;

        if(event->modifiers() & Qt::ControlModifier)
        {
            mul = 10.0f;
        }

        Vec3 pos = view->position();

        pos.z += float(event->delta() / 120) * mul;
        if(pos.z > -1.0f) pos.z = -1.0f;

        view->setPositionImmediate(pos);
    }
}
