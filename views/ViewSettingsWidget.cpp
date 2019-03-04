#include "ViewSettingsWidget.h"

#include "style/Style.h"

#include "gui/PanelComboBox.h"
#include "gui/PanelSeparator.h"
#include "gui/PanelButton.h"
#include "gui/RowWidget.h"

#include "views/ModelView.h"

#include <QtWidgets/QFormLayout>
#include <QtWidgets/QCheckBox>

ViewSettingsWidget::ViewSettingsWidget(QWidget *parent)
    : QAbstractScrollArea(parent)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Base, Style::baseColor().darker(110));

    setPalette(pal);

    shadeCombo = new PanelComboBox();
    shadeCombo->addItem("Flat", Shade::Flat);
    shadeCombo->addItem("Smooth", Shade::Smooth);
    shadeCombo->addItem("None", Shade::None);

    connect(shadeCombo, SIGNAL(currentIndexChanged(int)), SLOT(shadingChanged(int)));

    wireframeCheck = new QCheckBox("Wireframe");
    wireframeCheck->setFocusPolicy(Qt::NoFocus);

    connect(wireframeCheck, SIGNAL(toggled(bool)), SLOT(wireframeToggled(bool)));

    projectionCombo = new PanelComboBox();
    projectionCombo->addItem("Perspective", Projection::Perspective);
    projectionCombo->addItem("Orthographic", Projection::Orthographic);

    connect(projectionCombo, SIGNAL(currentIndexChanged(int)), SLOT(projectionChanged(int)));

    jointsCheck = new QCheckBox("Draw Joints");
    jointsCheck->setFocusPolicy(Qt::NoFocus);

    connect(jointsCheck, SIGNAL(toggled(bool)), SLOT(jointsToggled(bool)));

    paintCombo = new PanelComboBox();
    paintCombo->addItem("None", Paint::None);
    paintCombo->addItem("Palette", Paint::Palette);
    paintCombo->addItem("Joints", Paint::Joints);

    connect(paintCombo, SIGNAL(currentIndexChanged(int)), SLOT(paintChanged(int)));

    reflectCombo = new PanelComboBox();
    reflectCombo->addItem("X", Axis::X);
    reflectCombo->addItem("Y", Axis::Y);
    reflectCombo->addItem("Z", Axis::Z);
    reflectCombo->addItem("None", Axis::None);

    connect(reflectCombo, SIGNAL(currentIndexChanged(int)), SLOT(reflectChanged(int)));

    smoothCheck = new QCheckBox("Smooth Edges");
    smoothCheck->setFocusPolicy(Qt::NoFocus);

    connect(smoothCheck, SIGNAL(toggled(bool)), SLOT(smoothToggled(bool)));

    QFormLayout *layout = new QFormLayout(this);
    layout->setMargin(12);

    layout->addRow("Shading", shadeCombo);
    layout->addRow("Projection", projectionCombo);
    layout->addRow("", wireframeCheck);

    layout->addRow(new PanelSeparator());

    layout->addRow("Paint", paintCombo);
    layout->addRow("", jointsCheck);

    layout->addRow(new PanelSeparator());

    layout->addRow("Reflect", reflectCombo);
    layout->addRow("", smoothCheck);

    layout->addRow(new PanelSeparator());

    PanelButton *resetButton = new PanelButton("Reset");
    resetButton->setMinimumWidth(70);

    connect(resetButton, SIGNAL(clicked()), SLOT(resetClicked()));

    RowWidget *row = new RowWidget(this);

    row->addStretch();
    row->addWidget(resetButton);

    layout->addWidget(row);
}

QSize
ViewSettingsWidget::sizeHint() const
{
    return QSize(210, 254);
}

void
ViewSettingsWidget::setModelView(ModelView *view)
{
    modelView = view;

    shadeCombo->setCurrentData(view->shading());
    wireframeCheck->setChecked(view->wireframeRender());
    projectionCombo->setCurrentData(view->projection());
    jointsCheck->setChecked(view->renderSkeleton());
    paintCombo->setCurrentData(view->painting());

    reflectCombo->setCurrentData(view->reflection());
    smoothCheck->setChecked(view->smoothReflectionEdges());
}

void
ViewSettingsWidget::shadingChanged(int index)
{
    modelView->setShading(static_cast<Shade::Type>(shadeCombo->itemData(index).toInt()));
}

void
ViewSettingsWidget::wireframeToggled(bool state)
{
    modelView->setWireframeRender(wireframeCheck->isChecked());
}

void
ViewSettingsWidget::projectionChanged(int index)
{
    modelView->setProjection(static_cast<Projection::Type>(projectionCombo->itemData(index).toInt()));
}

void
ViewSettingsWidget::jointsToggled(bool state)
{
    modelView->setRenderSkeleton(jointsCheck->isChecked());
}

void
ViewSettingsWidget::paintChanged(int index)
{
    modelView->setPainting(static_cast<Paint::Type>(paintCombo->itemData(index).toInt()));
}

void
ViewSettingsWidget::reflectChanged(int index)
{
    modelView->setReflection(static_cast<Axis::Type>(reflectCombo->itemData(index).toInt()));
}

void
ViewSettingsWidget::smoothToggled(bool state)
{
    modelView->setSmoothReflectionEdges(state);
}

void
ViewSettingsWidget::resetClicked()
{
    modelView->reset();
    setModelView(modelView);
}
