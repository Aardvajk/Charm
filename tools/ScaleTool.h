#ifndef SCALETOOL_H
#define SCALETOOL_H

#include "tools/BaseModelViewTool.h"

#include "common/ScopedPtr.h"

class ActionList;
class AxisWidget;
class TransformCommand;
class PanelComboBox;
class LineHandle;
class Settings;

class ScaleTool : public BaseModelViewTool
{
    Q_OBJECT

public:
    ScaleTool(Settings *settings, ActionList *actions, Model *model, ModelViewGrid *viewGrid, QObject *parent = 0);
    virtual ~ScaleTool();

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
    PanelComboBox *typeBox;

    LineHandle *handle;

    TransformCommand *command;
    QPointF start;

    Vec3 point;
};

#endif // SCALETOOL_H
