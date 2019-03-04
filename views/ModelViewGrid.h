#ifndef MODELVIEWGRID_H
#define MODELVIEWGRID_H

#include <QtWidgets/QWidget>

class Graphics;
class Model;
class ModelBuffers;
class ModelView;
class ModelWidget;
class ActionList;
class PopUpWindow;
class Settings;

class ModelViewGrid : public QWidget
{
    Q_OBJECT

public:
    ModelViewGrid(Settings *settings, ActionList *actions, Graphics *graphics, Model *model, ModelBuffers *buffers, QWidget *parent = 0);

signals:
    void
    mousePressed(ModelView *view, QMouseEvent *event);

    void
    mouseMoved(ModelView *view, QMouseEvent *event);

    void
    mouseReleased(ModelView *view, QMouseEvent *event);

    void
    mouseWheelEvent(ModelView *view, QWheelEvent *event);

    void
    render(ModelView *view, Graphics *graphics);

public slots:
    void
    updateViews();

    void
    savingSettings();

protected:
    virtual
    void
    resizeEvent(QResizeEvent *event);

private slots:
    void
    maximizeToggle(ModelWidget *widget);

    void
    widgetActivated();

    void
    resetAllViews();

private:
    void
    arrangeWidgets();

    Settings *settings;

    ModelWidget *widgets[4];
    int maxWidget;

    ModelWidget *activeWidget;
};

#endif // MODELVIEWGRID_H
