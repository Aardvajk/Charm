#include "MoveTool.h"

#include "settings/Settings.h"

#include "actions/Action.h"
#include "actions/ActionList.h"

#include "model/Model.h"

#include "views/ModelView.h"

#include "gui/AxisWidget.h"
#include "gui/PanelComboBox.h"

#include "command/TransformCommand.h"
#include "command/MoveCommand.h"
#include "command/MoveCursorCommand.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtGui/QMouseEvent>

MoveTool::MoveTool(Settings *settings, ActionList *actions, Model *model, ModelViewGrid *viewGrid, QObject *parent)
    : BaseModelViewTool(settings, model, viewGrid, parent),
      options(new QWidget()),
      transformCommand(0),
      moveCommand(0),
      moveCursorCommand(0)
{
    Settings *child = settings->child("Tools")->child("Move");

    Action *moveAction = actions->add("Tools", "Move");
    moveAction->setShortcut(QKeySequence("M"));

    connect(moveAction, SIGNAL(triggered()), SIGNAL(selected()));

    addRelatedAction(moveAction);

    QVBoxLayout *layout = new QVBoxLayout(options.get());
    layout->setMargin(0);

    axisWidget = new AxisWidget(model, "Move", true, true);
    connect(axisWidget, SIGNAL(actionClicked()), SLOT(axisActionClicked()));

    axisWidget->loadSettings(child->child("axis"));

    typeCombo = new PanelComboBox();
    typeCombo->addItem("Selection", 0);
    typeCombo->addItem("Cursor", 1);

    typeCombo->setCurrentData(child->child("Type")->value(0));

    axisWidget->addWidget(typeCombo);

    layout->addWidget(new QLabel("Move"));
    layout->addWidget(axisWidget);

    connect(model, SIGNAL(modeChanged()), SLOT(modeChanged()));
}

MoveTool::~MoveTool()
{
}

QPixmap
MoveTool::image() const
{
    return QPixmap(":/resources/images/movetool.png");
}

QString
MoveTool::name() const
{
    return "Move";
}

QWidget*
MoveTool::optionsWidget()
{
    return options.get();
}

void
MoveTool::focusLost()
{
    endCommands();
}

void
MoveTool::mousePressed(ModelView *view, QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && !(event->modifiers() & Qt::ControlModifier))
    {
        if(typeCombo->currentData().toInt() == 1)
        {
            moveCursorCommand = new MoveCursorCommand(model(), this);
            moveCursorCommand->begin(view->getWorldPosition(event->pos(), model()->cursor()));
        }
        else if(model()->mode() == Mode::Model)
        {
            transformCommand = new TransformCommand("Move", model(), this);

            transformCommand->setVertices(model()->verticesInSelection());
            transformCommand->setJoints(model()->jointsInSelection());
        }
        else
        {
            moveCommand = new MoveCommand(model(), this);

            moveCommand->setJoints(model()->selectedJoints());
        }

        startPos = event->pos();
    }
    else
    {
        BaseModelViewTool::mousePressed(view, event);
    }
}

void
MoveTool::mouseMoved(ModelView *view, QMouseEvent *event)
{
    if(transformCommand || moveCommand || moveCursorCommand)
    {
        Vec3 line = view->getViewPoint(event->pos()) - view->getViewPoint(startPos);

        if(transformCommand)
        {
            transformCommand->transform(filterMatrix(translationMatrix(line), Vec3(1, 1, 1), axisWidget->filter()));
        }
        else if(moveCommand)
        {
            moveCommand->transform(filterMatrix(translationMatrix(line), Vec3(1, 1, 1), axisWidget->filter()));
        }
        else
        {
            moveCursorCommand->update(view->getWorldPosition(event->pos(), model()->cursor()));
        }
    }

    BaseModelViewTool::mouseMoved(view, event);
}

void
MoveTool::mouseReleased(ModelView *view, QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        endCommands();
    }

    BaseModelViewTool::mouseReleased(view, event);
}

void
MoveTool::saveSettings(Settings *settings) const
{
    Settings *child = settings->child("Tools")->child("Move");

    child->clear();
    axisWidget->saveSettings(child->child("axis"));
    child->child("Type")->setValue(typeCombo->currentData().toInt());
}

void
MoveTool::axisActionClicked()
{
    if(typeCombo->currentData().toInt() == 1)
    {
        moveCursorCommand = new MoveCursorCommand(model(), this);

        if(axisWidget->transformType() == AxisWidget::Absolute)
        {
            Vec3 pos = model()->cursor();

            if(axisWidget->filter().x) pos.x = axisWidget->values().x;
            if(axisWidget->filter().y) pos.y = axisWidget->values().y;
            if(axisWidget->filter().z) pos.z = axisWidget->values().z;

            moveCursorCommand->begin(pos);
        }
        else
        {
            moveCursorCommand->begin(model()->cursor() + mulVectors(axisWidget->values(), axisWidget->filter()));
        }

        moveCursorCommand->finalise();

        model()->endCommand(moveCursorCommand);
        moveCursorCommand = 0;
    }
    else if(model()->mode() == Mode::Model)
    {
        transformCommand = new TransformCommand("Move", model(), this);

        transformCommand->setVertices(model()->verticesInSelection());
        transformCommand->setJoints(model()->jointsInSelection());

        if(axisWidget->transformType() == AxisWidget::Absolute)
        {
            transformCommand->transform(translationMatrix(mulVectors(axisWidget->values() - model()->selectionCentre(), axisWidget->filter())));
        }
        else
        {
            transformCommand->transform(translationMatrix(axisWidget->values()));
        }

        transformCommand->finalise();

        model()->endCommand(transformCommand);
        transformCommand = 0;
    }
    else
    {
        moveCommand = new MoveCommand(model(), this);

        moveCommand->setJoints(model()->selectedJoints());

        if(axisWidget->transformType() == AxisWidget::Absolute)
        {
            moveCommand->transform(translationMatrix(mulVectors(axisWidget->values() - model()->selectionCentre(), axisWidget->filter())));
        }
        else
        {
            moveCommand->transform(translationMatrix(axisWidget->values()));
        }

        moveCommand->finalise();

        model()->endCommand(moveCommand);
        moveCommand = 0;
    }
}

void
MoveTool::modeChanged()
{
    emit enabled(true);
}

void
MoveTool::endCommands()
{
    if(transformCommand)
    {
        transformCommand->finalise();

        model()->endCommand(transformCommand);
        transformCommand = 0;
    }

    if(moveCommand)
    {
        moveCommand->finalise();

        model()->endCommand(moveCommand);
        moveCommand = 0;
    }

    if(moveCursorCommand)
    {
        moveCursorCommand->finalise();

        model()->endCommand(moveCursorCommand);
        moveCursorCommand = 0;
    }
}
