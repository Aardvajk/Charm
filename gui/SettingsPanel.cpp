#include "SettingsPanel.h"

#include "style/Style.h"

#include "actions/Action.h"
#include "actions/ActionList.h"

#include "gui/AnchorWidget.h"
#include "gui/AxisLockWidget.h"
#include "gui/PanelButton.h"

#include "model/Model.h"

#include <QtWidgets/QHBoxLayout>
#include <QtGui/QPainter>
#include <QtGui/QLinearGradient>

SettingsPanel::SettingsPanel(Model *model, ActionList *actions, QWidget *parent)
    : QWidget(parent),
      model(model)
{
    setMinimumHeight(34);
    setMaximumHeight(minimumHeight());

    setMaximumWidth(240);
    setMinimumWidth(240);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(6);

    anchWidget = new AnchorWidget(actions, model);
    axisWidget = new AxisLockWidget(actions, model);

    PanelButton *cursorButton = new PanelButton("C");
    cursorButton->setCheckable(true);
    cursorButton->setDown(false);

    connect(actions->add("Settings", "Toggle Cursor"), SIGNAL(triggered()), cursorButton, SLOT(toggle()));
    connect(cursorButton, SIGNAL(toggled(bool)), model, SLOT(setAlwaysShowCursor(bool)));

    PanelButton *normalButton = new PanelButton("N");
    normalButton->setCheckable(true);
    normalButton->setDown(false);

    connect(actions->add("Settings", "Toggle Normals"), SIGNAL(triggered()), normalButton, SLOT(toggle()));
    connect(normalButton, SIGNAL(toggled(bool)), model, SLOT(setShowNormals(bool)));

    layout->addWidget(anchWidget);
    layout->addWidget(cursorButton);
    layout->addWidget(normalButton);
    layout->addStretch();
    layout->addWidget(axisWidget);
}

void
SettingsPanel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QColor color = Style::baseColor();

    QLinearGradient g(rect().topLeft(), rect().bottomLeft());
    g.setColorAt(0, color);
    g.setColorAt(1, color.darker(120));

    painter.fillRect(rect(), g);
}

