#include "RotateTool.h"

#include "maths/Maths.h"

#include "settings/Settings.h"

#include "actions/Action.h"
#include "actions/ActionList.h"

#include "model/Model.h"

#include "views/ModelView.h"
#include "views/ModelViewGrid.h"

#include "gui/AxisWidget.h"

#include "command/TransformCommand.h"
#include "command/RotateCommand.h"

#include "tools/visuals/LineHandle.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtGui/QMouseEvent>

RotateTool::RotateTool(Settings *settings, ActionList *actions, Model *model, ModelViewGrid *viewGrid, QObject *parent)
    : BaseModelViewTool(settings, model, viewGrid, parent),
      options(new QWidget()),
      moveCommand(0),
      rotateCommand(0)
{
    Settings *child = settings->child("Tools")->child("Rotate");

    Action *rotateAction = actions->add("Tools", "Rotate");
    rotateAction->setShortcut(QKeySequence("R"));

    connect(rotateAction, SIGNAL(triggered()), SIGNAL(selected()));

    addRelatedAction(rotateAction);

    handle = new LineHandle(this);

    QVBoxLayout *layout = new QVBoxLayout(options.get());
    layout->setMargin(0);

    axisWidget = new AxisWidget(model, "Rotate", false, false);
    connect(axisWidget, SIGNAL(actionClicked()), SLOT(axisActionClicked()));

    axisWidget->loadSettings(child->child("axis"));

    layout->addWidget(new QLabel("Rotate"));
    layout->addWidget(axisWidget);

    connect(viewGrid, SIGNAL(render(ModelView*,Graphics*)), handle, SLOT(render(ModelView*,Graphics*)));

    connect(model, SIGNAL(modeChanged()), SLOT(modeChanged()));
}

QPixmap
RotateTool::image() const
{
    return QPixmap(":/resources/images/rotatetool.png");
}

QString
RotateTool::name() const
{
    return "Rotate";
}

QWidget*
RotateTool::optionsWidget() const
{
    return options.get();
}

void
RotateTool::focusLost()
{
    endCommands();
    handle->setActiveView(0);
}

void
RotateTool::mousePressed(ModelView *view, QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && !(event->modifiers() & Qt::ControlModifier))
    {
        if(!model()->verticesInSelection().isEmpty() || !model()->jointsInSelection().isEmpty())
        {
            point = model()->referencePoint();

            Matrix m = view->transform();
            QPointF c = normalizedPointToScreenCoord(transformCoord(point, m), view->size());

            handle->setAnchor(c.toPoint());
            handle->setPosition(event->pos());
            handle->setActiveView(view);

            if(model()->mode() == Mode::Model)
            {
                moveCommand = new TransformCommand("Rotate", model(), this);

                moveCommand->setVertices(model()->verticesInSelection());
                moveCommand->setJoints(model()->jointsInSelection());
            }
            else
            {
                rotateCommand = new RotateCommand(model(), this);

                rotateCommand->setJoints(model()->selectedJoints());
            }

            startAngle = atan2(event->pos().y() - c.y(), event->pos().x() - c.x());
        }
    }
    else
    {
        BaseModelViewTool::mousePressed(view, event);
    }
}

void
RotateTool::mouseMoved(ModelView *view, QMouseEvent *event)
{
    if(moveCommand || rotateCommand)
    {
        Matrix t0 = translationMatrix(point * -1.0f);
        Matrix t1 = translationMatrix(point);

        Vec3 a = transformNormal(Vec3(0, 0, 1), view->worldTransform());
        a.z *= -1.0f;

        Matrix m = view->transform();
        QPointF c = normalizedPointToScreenCoord(transformCoord(point, m), view->size());

        float angle = atan2(event->pos().y() - c.y(), event->pos().x() - c.x());

        handle->setPosition(event->pos());

        if(moveCommand)
        {
            m = t0 * rotationAxisMatrix(a, angle - startAngle) * t1;
            moveCommand->transform(m);
        }

        if(rotateCommand)
        {
            rotateCommand->transform(a, angle - startAngle);
        }
    }

    BaseModelViewTool::mouseMoved(view, event);
}

void
RotateTool::mouseReleased(ModelView *view, QMouseEvent *event)
{
    endCommands();

    handle->setActiveView(0);
    BaseModelViewTool::mouseReleased(view, event);
}

void
RotateTool::saveSettings(Settings *settings) const
{
    Settings *child = settings->child("Tools")->child("Rotate");

    child->clear();
    axisWidget->saveSettings(child->child("axis"));
}

void
RotateTool::axisActionClicked()
{
    Matrix r = rotationMatrixDegrees(-axisWidget->values());

    Matrix t0 = translationMatrix(model()->referencePoint() * -1.0f);
    Matrix t1 = translationMatrix(model()->referencePoint());

    Matrix m = t0 * r * t1;

    if(model()->mode() == Mode::Model)
    {
        moveCommand = new TransformCommand("Rotate", model(), this);

        moveCommand->setVertices(model()->verticesInSelection());
        moveCommand->setJoints(model()->jointsInSelection());

        moveCommand->transform(m);
        moveCommand->finalise();

        model()->endCommand(moveCommand);
        moveCommand = 0;
    }
    else
    {
        rotateCommand = new RotateCommand(model(), this);

        rotateCommand->setJoints(model()->selectedJoints());

        rotateCommand->transform(degreesToRadians(axisWidget->values()));
        rotateCommand->finalise();

        model()->endCommand(rotateCommand);
        rotateCommand = 0;
    }
}

void
RotateTool::modeChanged()
{
    emit enabled(true);
}

void
RotateTool::endCommands()
{
    if(moveCommand)
    {
        moveCommand->finalise();

        model()->endCommand(moveCommand);
        moveCommand = 0;
    }

    if(rotateCommand)
    {
        rotateCommand->finalise();

        model()->endCommand(rotateCommand);
        rotateCommand = 0;
    }
}
