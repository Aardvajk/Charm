#ifndef TOOLPANEL_H
#define TOOLPANEL_H

#include <QtWidgets/QWidget>

class QVBoxLayout;
class Tool;
class ToolPanelButton;

class ToolPanel : public QWidget
{
    Q_OBJECT

public:
    ToolPanel(QWidget *parent = 0);

    Tool*
    addTool(Tool *tool);

    QList<Tool*>
    tools() const;

    Tool*
    tool() const;

protected:
    virtual
    void
    paintEvent(QPaintEvent *event);

    virtual
    bool
    eventFilter(QObject *object, QEvent *event);

signals:
    void
    toolChanged(Tool *tool, Tool *previous);

private slots:
    void
    buttonPressed();

private:
    QVBoxLayout *layout;

    ToolPanelButton *current;
    QVector<ToolPanelButton*> buttons;
};

#endif // TOOLPANEL_H
