#include "VertexPrimitive.h"

#include <QtGui/QMouseEvent>

#include "model/Model.h"

#include "command/CreateCommand.h"

#include "views/ModelView.h"

VertexPrimitive::VertexPrimitive(Model *model, QObject *parent)
    : Primitive(model, parent)
{
}

QString
VertexPrimitive::name() const
{
    return "Vertex";
}

void
VertexPrimitive::create(const Vec3 &pos)
{
    createVertex(pos, false);
}

void
VertexPrimitive::mousePressed(ModelView *view, QMouseEvent *event)
{
    createVertex(view->getWorldPosition(event->pos(), model()->referencePoint()), event->modifiers() & Qt::ShiftModifier);
}

void
VertexPrimitive::createVertex(const Vec3 &pos, bool preserve)
{
    CreateCommand *command = new CreateCommand("Add Vertex", model(), this);

    command->setVertex(Vertex(pos, true));
    command->activate(preserve);

    model()->endCommand(command);
}
