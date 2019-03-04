#include "AnimationOperations.h"

#include "actions/Action.h"
#include "actions/ActionList.h"

#include "model/Model.h"

#include "command/ModifyCommand.h"

AnimationOperations::AnimationOperations(ActionList *actions, Model *model, QObject *parent)
    : QObject(parent),
      model(model)
{
    resetAction = actions->add("Animation", "Reset");
    resetAction->setText("&Reset to Bind Pose");

    resetAction->setEnabled(false);

    connect(resetAction, SIGNAL(triggered()), SLOT(reset()));

    swapSelectedAction = actions->add("Animation", "Swap Selected Transforms");
    swapSelectedAction->setText("&Swap Selected Pair Transformations");

    swapSelectedAction->setEnabled(false);

    connect(swapSelectedAction, SIGNAL(triggered()), SLOT(swapSelected()));

    swapAllAction = actions->add("Animation", "Swap All Transforms");
    swapAllAction->setText("Swap &All Pair Transformations");

    swapAllAction->setEnabled(false);

    connect(swapAllAction, SIGNAL(triggered()), SLOT(swapAll()));

    connect(model, SIGNAL(modeChanged()), SLOT(modeChanged()));
}

void
AnimationOperations::modeChanged()
{
    bool enabled = model->mode() == Mode::Animate;

    resetAction->setEnabled(enabled);
    swapSelectedAction->setEnabled(enabled);
    swapAllAction->setEnabled(enabled);
}

void
AnimationOperations::reset()
{
    QVector<StoredJoint> joints;

    for(int i = 0; i < model->jointCount(); ++i)
    {
        Joint joint = model->joint(i);
        joint.transform = JointTransform();

        joints.append(StoredJoint(i, model->joint(i), joint));
    }

    ModifyCommand *command = new ModifyCommand("Reset Pose", model, this);

    command->setJoints(joints);
    command->activate();

    model->endCommand(command);
}

void
AnimationOperations::swapSelected()
{
    swap(true);
}

void
AnimationOperations::swapAll()
{
    swap(false);
}

void
AnimationOperations::swap(bool onlySelected)
{
    QSet<int> jointSet;
    QSet<int> pairSet;

    for(int i = 0; i < model->jointCount(); ++i)
    {
        if((!onlySelected || model->joint(i).selected) && model->joint(i).details.paired != -1)
        {
            pairSet.insert(model->joint(i).details.paired);
            if(!pairSet.contains(i))
            {
                jointSet.insert(i);
            }
        }
    }

    QVector<StoredJoint> joints;

    foreach(int i, jointSet)
    {
        int j = model->joint(i).details.paired;

        Joint a = model->joint(i);
        Joint b = model->joint(j);

        qSwap(a.transform, b.transform);

        a.transform.rotation.x *= -1;
        a.transform.rotation.w *= -1;

        b.transform.rotation.x *= -1;
        b.transform.rotation.w *= -1;

        joints.append(StoredJoint(i, model->joint(i), a));
        joints.append(StoredJoint(j, model->joint(j), b));
    }

    ModifyCommand *command = new ModifyCommand("Swap Transforms", model, this);

    command->setJoints(joints);
    command->activate();

    model->endCommand(command);
}
