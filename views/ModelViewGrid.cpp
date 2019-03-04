#include "ModelViewGrid.h"

#include "settings/Settings.h"

#include "actions/Action.h"
#include "actions/ActionList.h"

#include "views/ModelView.h"
#include "views/ModelWidget.h"

#include "gui/PopUpWindow.h"
#include "gui/PanelListBox.h"

#include "common/Rotation.h"

#include "debug/Assert.h"

ModelViewGrid::ModelViewGrid(Settings *settings, ActionList *actions, Graphics *graphics, Model *model, ModelBuffers *buffers, QWidget *parent)
    : QWidget(parent),
      settings(settings),
      maxWidget(-1)
{
    for(int i = 0; i < 4; ++i)
    {
        widgets[i] = new ModelWidget(graphics, model, buffers, this);

        connect(widgets[i]->view(), SIGNAL(mousePressed(ModelView*,QMouseEvent*)), SIGNAL(mousePressed(ModelView*,QMouseEvent*)));
        connect(widgets[i]->view(), SIGNAL(mouseMoved(ModelView*,QMouseEvent*)), SIGNAL(mouseMoved(ModelView*,QMouseEvent*)));
        connect(widgets[i]->view(), SIGNAL(mouseReleased(ModelView*,QMouseEvent*)), SIGNAL(mouseReleased(ModelView*,QMouseEvent*)));
        connect(widgets[i]->view(), SIGNAL(mouseWheelEvent(ModelView*,QWheelEvent*)), SIGNAL(mouseWheelEvent(ModelView*,QWheelEvent*)));

        connect(widgets[i]->view(), SIGNAL(render(ModelView*,Graphics*)), SIGNAL(render(ModelView*,Graphics*)));

        connect(widgets[i], SIGNAL(maximizeToggle(ModelWidget*)), SLOT(maximizeToggle(ModelWidget*)));
        connect(widgets[i], SIGNAL(activated()), SLOT(widgetActivated()));
    }

    widgets[0]->view()->setRotationImmediate(Rotation::Front);
    widgets[1]->view()->setRotationImmediate(Rotation::Left);
    widgets[2]->view()->setRotationImmediate(Rotation::Top);

    for(int i = 0; i < 3; ++i)
    {
        widgets[i]->view()->setProjection(Projection::Orthographic);
        widgets[i]->view()->setShading(Shade::None);
        widgets[i]->view()->setPainting(Paint::None);
    }

    widgets[3]->view()->setWireframeRender(false);
    widgets[3]->view()->setPainting(Paint::Palette);

    for(int i = 0; i < 4; ++i)
    {
        widgets[i]->view()->setCurrentSettingsAsDefault();
    }

    for(int i = 0; i < 4; ++i)
    {
        if(settings->child("Views")->contains(QString("View%1").arg(i)))
        {
            widgets[i]->view()->loadSettings(settings->child("Views")->child(QString("View%1").arg(i)));
        }
    }

    Action *resetAllAction = actions->add("View", "Reset All");
    resetAllAction->setText("&Reset All Views");

    connect(resetAllAction, SIGNAL(triggered()), SLOT(resetAllViews()));

    activeWidget = widgets[0];

    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    arrangeWidgets();
}

void
ModelViewGrid::updateViews()
{
    for(int i = 0; i < 4; ++i)
    {
        widgets[i]->view()->update();
    }
}

void
ModelViewGrid::savingSettings()
{
    settings->child("Views")->clear();

    for(int i = 0; i < 4; ++i)
    {
        widgets[i]->view()->saveSettings(settings->child("Views")->child(QString("View%1").arg(i)));
    }
}

void
ModelViewGrid::resizeEvent(QResizeEvent *event)
{
    arrangeWidgets();
}

void
ModelViewGrid::maximizeToggle(ModelWidget *widget)
{
    if(maxWidget != -1)
    {
        for(int i = 0; i < 4; ++i)
        {
            if(i != maxWidget)
            {
                widgets[i]->setVisible(true);
            }
        }

        maxWidget = -1;
    }
    else
    {
        for(int i = 0; i < 4; ++i)
        {
            if(widgets[i] == widget)
            {
                maxWidget = i;
                widgets[i]->setVisible(true);
            }
            else
            {
                widgets[i]->setVisible(false);
            }
        }
    }

    arrangeWidgets();
}

void
ModelViewGrid::widgetActivated()
{
    activeWidget = static_cast<ModelWidget*>(sender());
}

void
ModelViewGrid::resetAllViews()
{
    for(int i = 0; i < 4; ++i)
    {
        widgets[i]->view()->reset();
    }
}

void
ModelViewGrid::arrangeWidgets()
{
    if(maxWidget >= 0)
    {
        Assert(maxWidget < 4);
        widgets[maxWidget]->setGeometry(rect());
    }
    else
    {
        int across = width() / 2;
        int down = height() / 2;

        int remainingAcross = width() - across;
        int remainingDown = height() - down;

        int marg = 1;

        widgets[0]->setGeometry(0, 0, across - marg, down - marg);
        widgets[1]->setGeometry(across, 0, remainingAcross, down - marg);
        widgets[2]->setGeometry(0, down, across - marg, remainingDown);
        widgets[3]->setGeometry(across, down, remainingAcross, remainingDown);
    }

    updateGeometry();
}
