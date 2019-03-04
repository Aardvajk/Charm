#include "ModelWidget.h"
#include "ViewSettingsWidget.h"

#include <QtWidgets/QVBoxLayout>

#include "model/Model.h"

#include "views/ModelView.h"

#include "gui/ToolBar.h"
#include "gui/ToolButton.h"
#include "gui/PopUpWindow.h"
#include "gui/PanelListBox.h"

ModelWidget::ModelWidget(Graphics *graphics, Model *model, ModelBuffers *buffers, QWidget *parent)
    : QWidget(parent),
      model(model)
{
    createSettingsWindow();
    createRotationWindow();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);

    ToolBar *toolBar = new ToolBar(this);

    connect(toolBar, SIGNAL(doubleClicked()), SLOT(toolBarDoubleClicked()));

    maximizeButton = new ToolButton(this);

    maximizeButton->addImage(QPixmap(":/resources/images/maximize.png"));
    maximizeButton->addImage(QPixmap(":/resources/images/restore.png"));

    connect(maximizeButton, SIGNAL(clicked()), SLOT(maximizeClicked()));

    settingsButton = new ToolButton(this);
    settingsButton->addImage(QPixmap(":/resources/images/settings.png"));

    connect(settingsButton, SIGNAL(pressed()), SLOT(settingsPressed()));

    rotationButton = new ToolButton(this);

    connect(rotationButton, SIGNAL(pressed()), SLOT(rotationPressed()));

    ToolButton *camToCurButton = new ToolButton(this);
    camToCurButton->addImage(QPixmap(":/resources/images/camera.png"));

    connect(camToCurButton, SIGNAL(pressed()), SLOT(cameraToCursor()));

    toolBar->addWidget(settingsButton);
    toolBar->addWidget(rotationButton);
    toolBar->addStretch();
    toolBar->addWidget(camToCurButton);
    toolBar->addWidget(maximizeButton);

    layout->addWidget(toolBar);

    modelView = new ModelView(graphics, model, buffers, this);
    connect(modelView, SIGNAL(settingsChanged()), SLOT(viewSettingsChanged()));

    layout->addWidget(modelView);

    setFocusPolicy(Qt::StrongFocus);

    viewSettingsChanged();
}

void
ModelWidget::focusInEvent(QFocusEvent *event)
{
    emit activated();
}

void
ModelWidget::settingsPressed()
{
    ToolButton *button = static_cast<ToolButton*>(sender());

    ViewSettingsWidget *widget = static_cast<ViewSettingsWidget*>(settingsWindow->widget());
    widget->setModelView(modelView);

    settingsWindow->move(button->mapToGlobal(QPoint(0, button->height() - 1)));
    settingsWindow->setSlideSizes(QSize(button->width(), 0), widget->sizeHint());

    settingsWindow->show();
}

void
ModelWidget::rotationPressed()
{
    ToolButton *button = static_cast<ToolButton*>(sender());

    PanelListBox *listBox = static_cast<PanelListBox*>(rotationWindow->widget());

    listBox->setCurrentItem(static_cast<int>(modelView->rotation()));

    rotationWindow->move(button->mapToGlobal(QPoint(0, button->height() - 1)));
    rotationWindow->setSlideSizes(QSize(button->width(), 0), QSize(80, listBox->heightForItems(listBox->count())));

    rotationWindow->show();
}

void
ModelWidget::rotationSelected(int index)
{
    rotationWindow->hide();
    modelView->setRotation(static_cast<Rotation::Type>(index));
}

void
ModelWidget::popupHidden()
{
    settingsButton->clearAnimation();
    rotationButton->clearAnimation();
}

void
ModelWidget::toolBarDoubleClicked()
{
    emit maximizeToggle(this);
}

void
ModelWidget::maximizeClicked()
{
    maximizeButton->setImageIndex(maximizeButton->imageIndex() ? 0 : 1);
    updateToolTips();

    emit maximizeToggle(this);
}
void
ModelWidget::viewSettingsChanged()
{
    rotationButton->setText(Rotation::toString(modelView->rotation()));
    updateToolTips();
}

void
ModelWidget::createSettingsWindow()
{
    ViewSettingsWidget *widget = new ViewSettingsWidget();
    settingsWindow = new PopUpWindow(widget, this);

    connect(settingsWindow, SIGNAL(hidden()), SLOT(popupHidden()));
}

void
ModelWidget::createRotationWindow()
{
    PanelListBox *listBox = new PanelListBox();
    rotationWindow = new PopUpWindow(listBox, this);

    for(int i = 0; i <= static_cast<int>(Rotation::Bottom); ++i)
    {
        listBox->addItem(Rotation::toString(static_cast<Rotation::Type>(i)));
    }

    connect(rotationWindow, SIGNAL(hidden()), SLOT(popupHidden()));
    connect(listBox, SIGNAL(clicked(int)), SLOT(rotationSelected(int)));
}

void
ModelWidget::updateToolTips()
{
    maximizeButton->setToolTip(maximizeButton->imageIndex() ? "Restore" : "Maximize");
}

void
ModelWidget::cameraToCursor()
{
    modelView->setPosition(model->cursor());
}
