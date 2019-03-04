#include "ReorderJointCommand.h"

#include "model/Model.h"
#include "model/ModelData.h"

ReorderJointCommand::ReorderJointCommand(Model *model, QObject *parent)
    : Command(model, parent),
      fromIndex(-1),
      toIndex(-1)
{
}

QString
ReorderJointCommand::name() const
{
    return "Reorder Joints";
}

bool
ReorderJointCommand::isValid() const
{
    return fromIndex != toIndex;
}

bool
ReorderJointCommand::doesModifyPersistentState() const
{
    return true;
}

void
ReorderJointCommand::undo()
{
    swap(toIndex, fromIndex);
}

void
ReorderJointCommand::redo()
{
    swap(fromIndex, toIndex);
}

void
ReorderJointCommand::activate(int from, int to)
{
    fromIndex = from;
    toIndex = to;

    swap(fromIndex, toIndex);
}

void
ReorderJointCommand::swap(int from, int to)
{
    qSwap(modelData()->joints[from], modelData()->joints[to]);

    for(int i = 0; i < modelData()->joints.count(); ++i)
    {
        if(modelData()->joints[i].details.paired == from)
        {
            modelData()->joints[i].details.paired = to;
        }
        else if(modelData()->joints[i].details.paired == to)
        {
            modelData()->joints[i].details.paired = from;
        }

        if(modelData()->joints[i].details.parent == from)
        {
            modelData()->joints[i].details.parent = to;
        }
        else if(modelData()->joints[i].details.parent == to)
        {
            modelData()->joints[i].details.parent = from;
        }
    }

    for(int i = 0; i < modelData()->vertices.count(); ++i)
    {
        Vertex &v = modelData()->vertices[i];

        for(int j = 0; j < v.maximumInfluences; ++j)
        {
            if(v.influences[j].joint == from)
            {
                v.influences[j].joint = to;
            }
            else if(v.influences[j].joint == to)
            {
                v.influences[j].joint = from;
            }
        }
    }

    for(int i = 0; i < modelData()->animations.count(); ++i)
    {
        Animation &a = modelData()->animations[i];

        for(int k = 0; k < a.keyFrameCount(); ++k)
        {
            KeyFrame kf = a.keyFrame(k);
            kf.swap(from, to);

            a.setKeyFrame(k, kf);
        }
    }

    modelData()->model()->change();
}
