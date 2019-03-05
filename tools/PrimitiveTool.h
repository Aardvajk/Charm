#ifndef PRIMITIVETOOL_H
#define PRIMITIVETOOL_H

#include "tools/BaseModelViewTool.h"

#include "common/ScopedPtr.h"

class ActionList;
class PanelListBox;
class Primitive;
class Settings;
class PropertyPanel;

class PrimitiveTool : public BaseModelViewTool
{
    Q_OBJECT

public:
    PrimitiveTool(Settings *settings, ActionList *actions, Model *model, ModelViewGrid *viewGrid, QObject *parent = 0);
    virtual ~PrimitiveTool();

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
    listChanged(int index);

    void
    undoTriggered();

    void
    modeChanged();

    void
    createClicked();

private:
    ScopedPtr<QWidget> options;

    PanelListBox *list;
    QVector<Primitive*> primitives;

    Primitive *current;

    PropertyPanel *propertyPanel;
};


#endif // PRIMITIVETOOL_H
