#ifndef MODELWIDGET_H
#define MODELWIDGET_H

#include <QtWidgets/QWidget>

class Graphics;
class Model;
class ModelBuffers;
class ModelView;
class ToolButton;
class PopUpWindow;

class ModelWidget : public QWidget
{
    Q_OBJECT

public:
    ModelWidget(Graphics *graphics, Model *model, ModelBuffers *buffers, QWidget *parent = 0);

    ModelView*
    view() const { return modelView; }

signals:
    void
    maximizeToggle(ModelWidget *widget);

    void
    activated();

protected:
    virtual
    void
    focusInEvent(QFocusEvent *event);

private slots:
    void
    settingsPressed();

    void
    rotationPressed();

    void
    rotationSelected(int index);

    void
    popupHidden();

    void
    toolBarDoubleClicked();

    void
    maximizeClicked();

    void
    viewSettingsChanged();

    void
    cameraToCursor();

private:
    void
    createSettingsWindow();

    void
    createRotationWindow();

    void
    updateToolTips();

    Model *model;
    ModelView *modelView;

    ToolButton *settingsButton;
    ToolButton *rotationButton;
    ToolButton *maximizeButton;

    PopUpWindow *settingsWindow;
    PopUpWindow *rotationWindow;
};

#endif // MODELWIDGET_H
