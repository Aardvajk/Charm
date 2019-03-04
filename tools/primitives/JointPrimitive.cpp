#include "JointPrimitive.h"

#include <QtGui/QMouseEvent>

#include "model/Model.h"

#include "command/CreateCommand.h"

#include "views/ModelView.h"

JointPrimitive::JointPrimitive(Model *model, QObject *parent)
    : Primitive(model, parent)
{
}

QString
JointPrimitive::name() const
{
    return "Joint";
}

void
JointPrimitive::create(const Vec3 &pos)
{
    createJoint(pos, false);
}

void
JointPrimitive::mousePressed(ModelView *view, QMouseEvent *event)
{
    createJoint(view->getWorldPosition(event->pos(), model()->referencePoint()), event->modifiers() & Qt::ShiftModifier);
}

void
JointPrimitive::createJoint(const Vec3 &pos, bool preserve)
{
    CreateCommand *command = new CreateCommand("Add Joint", model(), this);

    Joint joint(pos, true);

    if(model()->selectedJoints().count() == 1)
    {
        joint.details.parent = model()->selectedJoints().front();
    }

    joint.details.name = QString("Joint %1").arg(model()->jointCount() + 1);

    command->setJoint(joint);
    command->activate(preserve);

    model()->endCommand(command);
}
