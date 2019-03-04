#include "WeightInfo.h"

#include "model/Model.h"

WeightInfo::WeightInfo(const VertexInfluence *in)
{
    for(int i = 0; i < Vertex::maximumInfluences; ++i)
    {
        influences[i] = in[i];
    }
}

bool
WeightInfo::operator!=(const WeightInfo &other) const
{
    for(int i = 0; i < Vertex::maximumInfluences; ++i)
    {
        if(influences[i] != other.influences[i])
        {
            return true;
        }
    }

    return false;
}

WeightInfo
WeightInfo::fromSelection(Model *model)
{
    if(model->selectedVertices().isEmpty())
    {
        return WeightInfo();
    }

    WeightInfo first(model->vertex(model->selectedVertices().front()).influences);
    bool equal = true;

    for(int i = 1; i < model->selectedVertices().count(); ++i)
    {
        WeightInfo other(model->vertex(model->selectedVertices()[i]).influences);

        if(first != other)
        {
            equal = false;
            break;
        }
    }

    if(equal)
    {
        return first;
    }

    return WeightInfo();
}
