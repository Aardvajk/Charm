#include "SupportPrimitive.h"

#include <QtGui/QMouseEvent>

#include "model/Model.h"

#include "command/CreateCommand.h"

#include "views/ModelView.h"

#include "support/SupportProvider.h"
#include "support/SupportShape.h"

SupportPrimitive::SupportPrimitive(Model *model, QObject *parent)
    : Primitive(model, parent)
{
}

QString
SupportPrimitive::name() const
{
    return "Support";
}

void
SupportPrimitive::create(const Vec3 &pos)
{
    createShape(pos, false);
}

void
SupportPrimitive::mousePressed(ModelView *view, QMouseEvent *event)
{
    createShape(view->getWorldPosition(event->pos(), model()->referencePoint()), event->modifiers() & Qt::ShiftModifier);
}

void
SupportPrimitive::createShape(const Vec3 &pos, bool preserve)
{
    SupportProvider provider;

    CreateCommand *command = new CreateCommand("Add Support", model(), this);

    createSupportShape(pos, provider, command, model()->vertexCount());

    command->activate(preserve);
    model()->endCommand(command);
}

