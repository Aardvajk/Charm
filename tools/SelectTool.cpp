#include "SelectTool.h"

#include "settings/Settings.h"

#include "actions/Action.h"
#include "actions/ActionList.h"

#include "model/Model.h"

#include "views/ModelView.h"
#include "views/ModelViewGrid.h"

#include "tools/visuals/BoxMarquee.h"

#include "command/SelectCommand.h"

#include "gui/ToggleBox.h"
#include "gui/Separator.h"

#include "common/ModelElement.h"

#include <QtGui/QMouseEvent>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>

SelectTool::SelectTool(Settings *settings, ActionList *actions, Model *model, ModelViewGrid *viewGrid, float jointRadius, QObject *parent)
    : BaseModelViewTool(settings, model, viewGrid, parent),
      options(new QWidget()),
      jointRadius(jointRadius)
{
    Settings *child = settings->child("Tools")->child("Select");

    Action *selectAction = actions->add("Tools", "Select");
    selectAction->setShortcut(QKeySequence("S"));

    connect(selectAction, SIGNAL(triggered()), SIGNAL(selected()));

    addRelatedAction(selectAction);

    marquee = new BoxMarquee(this);

    QVBoxLayout *layout = new QVBoxLayout(options.get());
    layout->setMargin(0);

    elementBox = new ToggleBox();
    elementBox->addItem(ModelElement::toString(ModelElement::Vertex), ModelElement::Vertex);
    elementBox->addItem(ModelElement::toString(ModelElement::Face), ModelElement::Face);
    elementBox->addItem(ModelElement::toString(ModelElement::Joint), ModelElement::Joint);

    connect(elementBox, SIGNAL(changed(QVariant,bool)), SLOT(elementsChanged(QVariant,bool)));

    details[Mode::Model].stored = child->child("modelElements")->value(QList<QVariant>() << ModelElement::Vertex).toList();
    details[Mode::Animate].stored = child->child("animateElements")->value(QList<QVariant>() << ModelElement::Joint).toList();

    frontOnlyCheck = new QCheckBox("Front faces only");
    details[Mode::Model].frontOnly = child->child("modelFrontonly")->value(false).toBool();
    details[Mode::Animate].frontOnly = child->child("animateFrontonly")->value(false).toBool();

    connect(frontOnlyCheck, SIGNAL(toggled(bool)), SLOT(frontOnlyToggled(bool)));

    layout->addWidget(new QLabel("Select"));
    layout->addWidget(elementBox);
    layout->addWidget(frontOnlyCheck);

    connect(viewGrid, SIGNAL(render(ModelView*,Graphics*)), marquee, SLOT(render(ModelView*,Graphics*)));

    elementsChanged(QVariant(), true);

    connect(model, SIGNAL(modeChanged()), SLOT(modeChanged()));
}

SelectTool::~SelectTool()
{
}

QPixmap
SelectTool::image() const
{
    return QPixmap(":/resources/images/selecttool.png");
}

QString
SelectTool::name() const
{
    return "Select";
}

QWidget*
SelectTool::optionsWidget()
{
    return options.get();
}

void
SelectTool::focusLost()
{
    marquee->setActiveView(0);
}

void
SelectTool::mousePressed(ModelView *view, QMouseEvent *event)
{
    if(!marquee->view() && (event->button() == Qt::LeftButton || event->button() == Qt::RightButton) && !(event->modifiers() & Qt::ControlModifier))
    {
        marquee->setAnchor(event->pos());
        marquee->setPosition(event->pos());
        marquee->setActiveView(view);

        startButton = event->button();
    }
    else
    {
        BaseModelViewTool::mousePressed(view, event);
    }
}

void
SelectTool::mouseMoved(ModelView *view, QMouseEvent *event)
{
    if(marquee->view() == view)
    {
        marquee->setPosition(event->pos());
    }

    BaseModelViewTool::mouseMoved(view, event);
}

void
SelectTool::mouseReleased(ModelView *view, QMouseEvent *event)
{
    if(event->button() == startButton)
    {
        marquee->setActiveView(0);

        SelectCommand *command = new SelectCommand(model(), this);

        if(startButton == Qt::LeftButton)
        {
            command->select(marquee->clipRect(view->size()), view->worldTransform(), view->transform(), event->modifiers() & Qt::ShiftModifier, elementBox->values(), frontOnlyCheck->isChecked(), jointRadius);
        }
        else if(startButton == Qt::RightButton)
        {
            command->deselect(marquee->clipRect(view->size()), view->worldTransform(), view->transform(), elementBox->values(), frontOnlyCheck->isChecked(), jointRadius);
        }

        model()->endCommand(command);
    }

    BaseModelViewTool::mouseReleased(view, event);
}

void
SelectTool::saveSettings(Settings *settings) const
{
    Settings *child = settings->child("Tools")->child("Select");

    child->clear();

    child->add("modelElements", details[Mode::Model].stored.toList());
    child->add("modelFrontonly", details[Mode::Model].frontOnly);

    child->add("animateElements", details[Mode::Animate].stored.toList());
    child->add("animateFrontonly", details[Mode::Animate].frontOnly);
}

void
SelectTool::elementsChanged(const QVariant &value, bool state)
{
    bool faces = elementBox->values().contains(ModelElement::Face);

    frontOnlyCheck->setEnabled(faces);

    details[model()->mode()].stored = elementBox->values();
}

void
SelectTool::frontOnlyToggled(bool state)
{
    details[model()->mode()].frontOnly = state;
}

void
SelectTool::modeChanged()
{
    elementBox->blockSignals(true);

    if(model()->mode() == Mode::Model)
    {
        elementBox->setValues(details[Mode::Model].stored);
        elementBox->setEnabledValues(VariantSet() << ModelElement::Vertex << ModelElement::Face << ModelElement::Joint);

        frontOnlyCheck->setChecked(details[Mode::Model].frontOnly);
    }
    else if(model()->mode() == Mode::Animate)
    {
        elementBox->setValues(details[Mode::Animate].stored);
        elementBox->setEnabledValues(VariantSet() << ModelElement::Vertex << ModelElement::Joint);

        frontOnlyCheck->setChecked(details[Mode::Animate].frontOnly);
    }

    elementBox->blockSignals(false);

    frontOnlyCheck->setEnabled(elementBox->values().contains(ModelElement::Face));

    emit enabled(true);
}
