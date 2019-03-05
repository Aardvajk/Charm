#include "ExtrudeTool.h"

#include "maths/Maths.h"

#include "settings/Settings.h"

#include "actions/Action.h"
#include "actions/ActionList.h"

#include "model/Model.h"

#include "views/ModelView.h"
#include "views/ModelViewGrid.h"

#include "gui/AxisWidget.h"
#include "gui/PanelComboBox.h"

#include "command/ExtrudeCommand.h"

#include "common/IndexPairKey.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtGui/QMouseEvent>

ExtrudeTool::ExtrudeTool(Settings *settings, ActionList *actions, Model *model, ModelViewGrid *viewGrid, QObject *parent)
    : BaseModelViewTool(settings, model, viewGrid, parent),
      options(new QWidget()),
      command(0)
{
    Settings *child = settings->child("Tools")->child("Extrude");

    Action *extrudeAction = actions->add("Tools", "Extrude");
    extrudeAction->setShortcut(QKeySequence("E"));

    connect(extrudeAction, SIGNAL(triggered()), SIGNAL(selected()));

    addRelatedAction(extrudeAction);

    QVBoxLayout *layout = new QVBoxLayout(options.get());
    layout->setMargin(0);

    axisWidget = new AxisWidget(model, "Extrude", true, true);
    connect(axisWidget, SIGNAL(actionClicked()), SLOT(axisActionClicked()));

    axisWidget->loadSettings(child->child("axis"));

    typeBox = new PanelComboBox();
    typeBox->addItem("Faces", ExtrudeCommand::Faces);
    typeBox->addItem("Edges", ExtrudeCommand::Edges);

    axisWidget->addWidget(typeBox);

    typeBox->setCurrentData(child->child("type")->value(ExtrudeCommand::Faces));

    layout->addWidget(new QLabel("Extrude"));
    layout->addWidget(axisWidget);

    connect(model, SIGNAL(modeChanged()), SLOT(modeChanged()));
}

ExtrudeTool::~ExtrudeTool()
{
}

QPixmap
ExtrudeTool::image() const
{
    return QPixmap(":/resources/images/extrudetool.png");
}

QString
ExtrudeTool::name() const
{
    return "Extrude";
}

QWidget*
ExtrudeTool::optionsWidget()
{
    return options.get();
}

void
ExtrudeTool::focusLost()
{
    endCommands();
}

void
ExtrudeTool::mousePressed(ModelView *view, QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && !(event->modifiers() & Qt::ControlModifier))
    {
        command = new ExtrudeCommand(static_cast<ExtrudeCommand::Type>(typeBox->currentData().toInt()), model(), this);
        command->setEdges(getEdges());

        startPos = event->pos();
    }
    else
    {
        BaseModelViewTool::mousePressed(view, event);
    }
}

void
ExtrudeTool::mouseMoved(ModelView *view, QMouseEvent *event)
{
    if(command)
    {
        Vec3 line = view->getViewPoint(event->pos()) - view->getViewPoint(startPos);
        command->transform(filterMatrix(translationMatrix(line), Vec3(1, 1, 1), axisWidget->filter()));
    }

    BaseModelViewTool::mouseMoved(view, event);
}

void
ExtrudeTool::mouseReleased(ModelView *view, QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && command)
    {
        endCommands();
    }

    BaseModelViewTool::mouseReleased(view, event);
}

void
ExtrudeTool::saveSettings(Settings *settings) const
{
    Settings *child = settings->child("Tools")->child("Extrude");

    child->clear();
    axisWidget->saveSettings(child->child("axis"));
    child->add("type", typeBox->currentData());
}

void
ExtrudeTool::axisActionClicked()
{
    command = new ExtrudeCommand(static_cast<ExtrudeCommand::Type>(typeBox->currentData().toInt()), model(), this);
    command->setEdges(getEdges());

    Vec3 v = axisWidget->values();

    if(axisWidget->transformType() == AxisWidget::Relative)
    {
        command->transform(translationMatrix(v));
    }
    else
    {
        Vec3 c = model()->selectionCentre();
        command->transform(filterMatrix(translationMatrix(v - c), Vec3(1, 1, 1), axisWidget->filter()));
    }

    model()->endCommand(command);
    command = 0;
}

void
ExtrudeTool::modeChanged()
{
    emit enabled(model()->mode() == Mode::Model);
}

QVector<Edge>
ExtrudeTool::getEdges() const
{
    if(static_cast<ExtrudeCommand::Type>(typeBox->currentData().toInt()) == ExtrudeCommand::Faces)
    {
        QMap<IndexPairKey, Edge> map;
        QMap<IndexPairKey, int> count;

        foreach(int i, model()->selectedFaces())
        {
            for(int j = 0; j < 3; ++j)
            {
                int k = (j < 2 ? j + 1 : 0);

                IndexPairKey key(model()->face(i).indices[j], model()->face(i).indices[k]);

                map[key] = Edge(i, j, k);
                ++count[key];
            }
        }

        QVector<Edge> edges;

        foreach(const IndexPairKey &key, map.keys())
        {
            if(count[key] == 1)
            {
                edges.append(map[key]);
            }
        }

        return edges;
    }
    else
    {
        QMap<IndexPairKey, Edge> map;
        QSet<IndexPairKey> used;

        foreach(int i, model()->selectedVertices())
        {
            foreach(int f, model()->facesContainingVertex(i))
            {
                for(int j = 0; j < 3; ++j)
                {
                    int k = (j < 2 ? j + 1 : 0);

                    if(model()->faceVertex(f, j).selected && model()->faceVertex(f, k).selected)
                    {
                        IndexPairKey key(model()->face(f).indices[j], model()->face(f).indices[k]);

                        if(!used.contains(key))
                        {
                            map[key] = Edge(f, j, k);
                            used.insert(key);
                        }
                    }
                }
            }
        }

        QVector<Edge> edges;

        for(QMap<IndexPairKey, Edge>::iterator i = map.begin(); i != map.end(); ++i)
        {
            edges.append(i.value());
        }

        return edges;
    }
}

void
ExtrudeTool::endCommands()
{
    if(command)
    {
        model()->endCommand(command);
        command = 0;
    }
}
