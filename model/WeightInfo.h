#ifndef WEIGHTINFO_H
#define WEIGHTINFO_H

#include "model/Vertex.h"

class Model;

class WeightInfo
{
public:
    WeightInfo(){ }
    WeightInfo(const VertexInfluence *in);

    bool
    operator!=(const WeightInfo &other) const;

    VertexInfluence influences[Vertex::maximumInfluences];

    static
    WeightInfo
    fromSelection(Model *model);
};

#endif // WEIGHTINFO_H
