#ifndef EXTRUDETOOL_H
#define EXTRUDETOOL_H

#include "tools/BaseModelViewTool.h"

#include "model/Edge.h"

#include "common/ScopedPtr.h"

class ActionList;
class AxisWidget;
class ExtrudeCommand;
class PanelComboBox;
class Settings;

class ExtrudeTool : public BaseModelViewTool
{
    Q_OBJECT

public:
    ExtrudeTool(Settings *settings, ActionList *actions, Model *model, ModelViewGrid *viewGrid, QObject *parent = 0);
    virtual ~ExtrudeTool();

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

    QVector<Edge>
    getEdges() const;

    ScopedPtr<QWidget> options;
    AxisWidget *axisWidget;
    PanelComboBox *typeBox;

    ExtrudeCommand *command;
    QPoint startPos;
};

#endif // EXTRUDETOOL_H
