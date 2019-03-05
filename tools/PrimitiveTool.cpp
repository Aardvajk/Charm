#include "PrimitiveTool.h"

#include "settings/Settings.h"

#include "actions/Action.h"
#include "actions/ActionList.h"

#include "model/Model.h"

#include "views/ModelView.h"

#include "command/CreateCommand.h"

#include "properties/PropertyPanel.h"

#include "gui/PanelListBox.h"
#include "gui/PanelButton.h"

#include "primitives/VertexPrimitive.h"
#include "primitives/JointPrimitive.h"
#include "primitives/CuboidPrimitive.h"
#include "primitives/SpherePrimitive.h"
#include "primitives/CylinderPrimitive.h"
#include "primitives/SupportPrimitive.h"

#include <QtGui/QMouseEvent>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>

PrimitiveTool::PrimitiveTool(Settings *settings, ActionList *actions, Model *model, ModelViewGrid *viewGrid, QObject *parent)
    : BaseModelViewTool(settings, model, viewGrid, parent),
      options(new QWidget()),
      current(0)
{
    Settings *child = settings->child("Tools")->child("Primitive");

    Action *primitiveAction = actions->add("Tools", "Primitive");
    primitiveAction->setShortcut(QKeySequence("P"));

    connect(primitiveAction, SIGNAL(triggered()), SIGNAL(selected()));

    addRelatedAction(primitiveAction);

    QVBoxLayout *layout = new QVBoxLayout(options.get());
    layout->setMargin(0);

    primitives.append(new VertexPrimitive(model, this));
    primitives.append(new JointPrimitive(model, this));
    primitives.append(new CuboidPrimitive(child, model, this));
    primitives.append(new SpherePrimitive(child, model, this));
    primitives.append(new CylinderPrimitive(child, model, this));
    primitives.append(new SupportPrimitive(model, this));

    list = new PanelListBox();
    list->setDrawSelection(true);

    layout->addWidget(new QLabel("Primitive"));
    layout->addWidget(list);

    propertyPanel = new PropertyPanel();
    layout->addWidget(propertyPanel);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setMargin(0);

    PanelButton *createButton = new PanelButton("Create");
    connect(createButton, SIGNAL(clicked()), SLOT(createClicked()));

    buttonLayout->addStretch();
    buttonLayout->addWidget(createButton);

    layout->addLayout(buttonLayout);

    connect(list, SIGNAL(currentChanged(int)), SLOT(listChanged(int)));

    foreach(const Primitive *primitive, primitives)
    {
        list->addItem(primitive->name());
    }

    if(list->count())
    {
        list->setCurrentItem(0);
    }

    list->setMaximumHeight(list->heightForItems(6));

    list->setCurrentItem(child->child("type")->value(0).toInt());

    connect(model, SIGNAL(undoTriggered()), SLOT(undoTriggered()));
    connect(model, SIGNAL(modeChanged()), SLOT(modeChanged()));
}

PrimitiveTool::~PrimitiveTool()
{
}

QPixmap
PrimitiveTool::image() const
{
    return QPixmap(":/resources/images/primitivetool.png");
}

QString
PrimitiveTool::name() const
{
    return "Primitive";
}

QWidget*
PrimitiveTool::optionsWidget()
{
    return options.get();
}

void
PrimitiveTool::focusLost()
{
    if(current)
    {
        current->focusLost();
    }
}

void
PrimitiveTool::mousePressed(ModelView *view, QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && !(event->modifiers() & Qt::ControlModifier))
    {
        if(current)
        {
            current->mousePressed(view, event);
        }
    }
    else
    {
        BaseModelViewTool::mousePressed(view, event);
    }
}

void
PrimitiveTool::mouseMoved(ModelView *view, QMouseEvent *event)
{
    if(current)
    {
        current->mouseMoved(view, event);
    }

    BaseModelViewTool::mouseMoved(view, event);
}

void
PrimitiveTool::mouseReleased(ModelView *view, QMouseEvent *event)
{
    if(current)
    {
        current->mouseReleased(view, event);
    }

    BaseModelViewTool::mouseReleased(view, event);
}

void
PrimitiveTool::saveSettings(Settings *settings) const
{
    Settings *child = settings->child("Tools")->child("Primitive");

    child->clear();
    child->add("type", list->currentItem());

    foreach(const Primitive *primitive, primitives)
    {
        primitive->saveSettings(child);
    }
}

void
PrimitiveTool::listChanged(int index)
{
    if(current)
    {
        current->focusLost();
    }

    current = primitives[index];

    QVector<Property*> properties = current->properties();

    propertyPanel->setProperties(properties);
    propertyPanel->setVisible(!properties.isEmpty());
}

void
PrimitiveTool::undoTriggered()
{
    if(current)
    {
        current->focusLost();
    }
}

void
PrimitiveTool::modeChanged()
{
    emit enabled(model()->mode() == Mode::Model);
}

void
PrimitiveTool::createClicked()
{
    if(current)
    {
        current->create(model()->referencePoint());
    }
}
