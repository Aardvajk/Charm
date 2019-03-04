#include "ToolOperations.h"

#include "actions/Action.h"
#include "actions/ActionList.h"

#include "model/Model.h"
#include "model/Edge.h"

#include "command/SelectCommand.h"

#include <QtCore/QMap>

ToolOperations::ToolOperations(ActionList *actions, Model *model, QObject *parent)
    : QObject(parent),
      model(model)
{
    Action *selectUnclosedAction = actions->add("Tools", "Select Unclosed");
    selectUnclosedAction->setText("Select &Unclosed Edges");

    connect(selectUnclosedAction, SIGNAL(triggered()), SLOT(selectUnclosed()));
}

void
ToolOperations::selectUnclosed()
{
    QMap<EdgeKey, Edge> edges;

    for(int f = 0; f < model->faceCount(); ++f)
    {
        Face face = model->face(f);

        for(int v0 = 0; v0 < 3; ++v0)
        {
            int v1 = (v0 < 2 ? v0 + 1 : 0);

            Edge newEdge;

            newEdge.indices[0] = face.indices[v0];
            newEdge.indices[1] = face.indices[v1];

            newEdge.face = f;

            EdgeKey key(face.indices[v0], face.indices[v1]);

            if(!edges.contains(key))
            {
                edges[key] = newEdge;
            }
            else
            {
                edges.remove(key);
            }
        }
    }

    QSet<int> vertexSet;
    foreach(const Edge &edge, edges)
    {
        vertexSet.insert(edge.indices[0]);
        vertexSet.insert(edge.indices[1]);
    }

    QVector<int> verts;
    foreach(int i, vertexSet)
    {
        verts.append(i);
    }

    SelectCommand *command = new SelectCommand(model, this);
    command->selectVertices(verts);

    model->endCommand(command);
}
