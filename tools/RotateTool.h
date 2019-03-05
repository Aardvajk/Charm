#ifndef ROTATETOOL_H
#define ROTATETOOL_H

#include "tools/BaseModelViewTool.h"

#include "common/ScopedPtr.h"

class ActionList;
class AxisWidget;
class LineHandle;
class TransformCommand;
class RotateCommand;
class Settings;

class RotateTool : public BaseModelViewTool
{
    Q_OBJECT

public:
    RotateTool(Settings *settings, ActionList *actions, Model *model, ModelViewGrid *viewGrid, QObject *parent = 0);
    virtual ~RotateTool();

    virtual
    QPixmap
    image() const;

    virtual
    QString
    name() const;

    virtual
    QWidget*
    optionsWidget();

    virtual
    void
    focusLost();

protected slots:
    virtual
    void
    mousePressed(ModelView *view, QMouseEvent *event);

    virtual
    void
    mouseMoved(ModelView *view, QMouseEvent *event);

    virtual
    void
    mouseReleased(ModelView *view, QMouseEvent *event);

protected:
    void
    saveSettings(Settings *settings) const;

private slots:
    void
    axisActionClicked();

    void
    modeChanged();

private:
    void
    endCommands();

    ScopedPtr<QWidget> options;
    AxisWidget *axisWidget;

    LineHandle *handle;

    TransformCommand *moveCommand;
    RotateCommand *rotateCommand;

    float startAngle;

    Vec3 point;
};

#endif // ROTATETOOL_H
