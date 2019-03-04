#include "WeightPanel.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>

#include "model/Model.h"
#include "model/WeightInfo.h"

#include "gui/WeightCombo.h"

#include "command/ModifyCommand.h"

WeightPanel::WeightPanel(Model *model, QWidget *parent)
    : QWidget(parent),
      model(model),
      ignoreChange(false)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    layout->addWidget(new QLabel("Weights"));
    layout->addSpacing(6);

    for(int i = 0; i < Vertex::maximumInfluences; ++i)
    {
        WeightCombo *combo = new WeightCombo(model);

        combos.append(combo);
        layout->addWidget(combo);

        if(i < Vertex::maximumInfluences - 1)
        {
            layout->addSpacing(2);
        }

        connect(combo, SIGNAL(changed()), SLOT(comboChanged()));
    }

    connect(model, SIGNAL(changed()), SLOT(modelChanged()));
    connect(model, SIGNAL(selectionChanged()), SLOT(modelChanged()));
}

void
WeightPanel::modelChanged()
{
    if(ignoreChange)
    {
        return;
    }

    WeightInfo info = WeightInfo::fromSelection(model);

    for(int i = 0; i < Vertex::maximumInfluences; ++i)
    {
        combos[i]->setJoint(info.influences[i].joint);
        combos[i]->setValue(info.influences[i].weight);
    }
}

void
WeightPanel::comboChanged()
{
    ignoreChange = true;

    float total = 0;
    for(int i = 0; i < Vertex::maximumInfluences; ++i)
    {
        total += combos[i]->value();
    }

    WeightInfo info;

    if(total != 0)
    {
        for(int i = 0; i < Vertex::maximumInfluences; ++i)
        {
            info.influences[i] = VertexInfluence(combos[i]->joint(), combos[i]->value() / total);
        }
    }

    QVector<StoredVertex> verts;

    foreach(int i, model->selectedVertices())
    {
        Vertex v = model->vertex(i);

        for(int j = 0; j < Vertex::maximumInfluences; ++j)
        {
            v.influences[j] = info.influences[j];
        }

        verts.append(StoredVertex(i, model->vertex(i), v));
    }

    ModifyCommand *command = new ModifyCommand("Edit Weights", model, this);

    command->setVertices(verts);
    command->activate();

    model->endCommand(command);

    ignoreChange = false;
}
