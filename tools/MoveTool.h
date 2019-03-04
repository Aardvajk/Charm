#ifndef MOVETOOL_H
#define MOVETOOL_H

#include "tools/BaseModelViewTool.h"

#include <memory>

class ActionList;
class AxisWidget;

class TransformCommand;
class MoveCommand;
class MoveCursorCommand;
class PanelComboBox;

class MoveTool : public BaseModelViewTool
{
    Q_OBJECT

public:
    MoveTool(Settings *settings, ActionList *actions, Model *model, ModelViewGrid *viewGrid, QObject *parent = 0);

    virtual
    QPixmap
    image() const;

    virtual
    QString
    name() const;

    virtual
    QWidget*
    optionsWidget() const;

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
    virtual
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

    std::auto_ptr<QWidget> options;
    AxisWidget *axisWidget;
    PanelComboBox *typeCombo;

    TransformCommand *transformCommand;
    MoveCommand *moveCommand;
    MoveCursorCommand *moveCursorCommand;

    QPoint startPos;
};

#endif // MOVETOOL_H
