#include "ScaleTool.h"

#include "settings/Settings.h"

#include "maths/Maths.h"

#include "actions/Action.h"
#include "actions/ActionList.h"

#include "model/Model.h"

#include "views/ModelView.h"
#include "views/ModelViewGrid.h"

#include "gui/AxisWidget.h"
#include "gui/PanelComboBox.h"

#include "command/TransformCommand.h"

#include "tools/visuals/LineHandle.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtGui/QMouseEvent>

ScaleTool::ScaleTool(Settings *settings, ActionList *actions, Model *model, ModelViewGrid *viewGrid, QObject *parent)
    : BaseModelViewTool(settings, model, viewGrid, parent),
      options(new QWidget()),
      command(0)
{
    Settings *child = settings->child("Tools")->child("Scale");

    Action *scaleAction = actions->add("Tools", "Scale");
    scaleAction->setShortcut(QKeySequence("K"));

    connect(scaleAction, SIGNAL(triggered()), SIGNAL(selected()));

    addRelatedAction(scaleAction);

    handle = new LineHandle(this);

    QVBoxLayout *layout = new QVBoxLayout(options.get());
    layout->setMargin(0);

    axisWidget = new AxisWidget(model, "Scale", false, false);
    axisWidget->setPlaceholder(1);
    connect(axisWidget, SIGNAL(actionClicked()), SLOT(axisActionClicked()));

    axisWidget->loadSettings(child->child("axis"));

    typeBox = new PanelComboBox();
    typeBox->addItem("Uniform", 1);
    typeBox->addItem("Free", 0);

    axisWidget->addWidget(typeBox);

    typeBox->setCurrentData(child->child("type")->value(1));

    layout->addWidget(new QLabel("Scale"));
    layout->addWidget(axisWidget);

    connect(viewGrid, SIGNAL(render(ModelView*,Graphics*)), handle, SLOT(render(ModelView*,Graphics*)));

    connect(model, SIGNAL(modeChanged()), SLOT(modeChanged()));
}

QPixmap
ScaleTool::image() const
{
    return QPixmap(":/resources/images/scaletool.png");
}

QString
ScaleTool::name() const
{
    return "Scale";
}

QWidget*
ScaleTool::optionsWidget() const
{
    return options.get();
}

void
ScaleTool::focusLost()
{
    endCommands();
    handle->setActiveView(0);
}

void
ScaleTool::mousePressed(ModelView *view, QMouseEvent *event)
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

            command = new TransformCommand("Scale", model(), this);

            command->setVertices(model()->verticesInSelection());
            command->setJoints(model()->jointsInSelection());

            start = event->pos();
        }
    }
    else
    {
        BaseModelViewTool::mousePressed(view, event);
    }
}

void
ScaleTool::mouseMoved(ModelView *view, QMouseEvent *event)
{
    if(command)
    {
        Matrix m = view->transform();
        QPointF c = normalizedPointToScreenCoord(transformCoord(point, m), view->size());

        handle->setPosition(event->pos());

        if(typeBox->currentData().toInt())
        {
            Matrix t0 = translationMatrix(point * -1.0f);
            Matrix t1 = translationMatrix(point);

            Matrix t = t0 * scalingMatrix(distance(c, event->pos()) / distance(c, start)) * t1;

            command->transform(filterMatrix(t, axisWidget->filter(), Vec3(1, 1, 1)));
        }
        else
        {
            float sx = fabs(c.x() - start.x());
            float sy = fabs(c.y() - start.y());

            float nx = fabs(c.x() - event->pos().x());
            float ny = fabs(c.y() - event->pos().y());

            Matrix t0 = translationMatrix(point * -1.0f);
            Matrix t1 = translationMatrix(point);

            Matrix w = view->worldTransform();
            Matrix iw = inverseMatrix(w);

            Matrix t = t0 * w * scalingMatrix(sx ? nx / sx : 0, sy ? ny / sy : 0, 1) * iw * t1;

            command->transform(filterMatrix(t, axisWidget->filter(), Vec3(1, 1, 1)));
        }
    }

    BaseModelViewTool::mouseMoved(view, event);
}

void
ScaleTool::mouseReleased(ModelView *view, QMouseEvent *event)
{
    endCommands();

    handle->setActiveView(0);
    BaseModelViewTool::mouseReleased(view, event);
}

void
ScaleTool::saveSettings(Settings *settings) const
{
    Settings *child = settings->child("Tools")->child("Scale");

    child->clear();
    axisWidget->saveSettings(child->child("axis"));
    child->add("type", typeBox->currentData());
}

void
ScaleTool::axisActionClicked()
{
    command = new TransformCommand("Scale", model(), this);

    command->setVertices(model()->verticesInSelection());
    command->setJoints(model()->jointsInSelection());

    Vec3 v = axisWidget->values(Vec3(1, 1, 1));

    Matrix t0 = translationMatrix(point * -1.0f);
    Matrix t1 = translationMatrix(point);

    command->transform(t0 * scalingMatrix(v) * t1);
    command->finalise();

    model()->endCommand(command);
    command = 0;
}

void
ScaleTool::modeChanged()
{
    emit enabled(model()->mode() == Mode::Model);
}

void
ScaleTool::endCommands()
{
    if(command)
    {
        command->finalise();

        model()->endCommand(command);
        command = 0;
    }
}
