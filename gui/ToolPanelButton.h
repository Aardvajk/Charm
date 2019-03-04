#ifndef TOOLPANELBUTTON_H
#define TOOLPANELBUTTON_H

#include <QtWidgets/QWidget>

class VariantAnimation;
class Tool;

class ToolPanelButton : public QWidget
{
    Q_OBJECT

public:
    ToolPanelButton(Tool *tool, QWidget *parent = 0);

    virtual
    QSize
    sizeHint() const;

    bool
    isDown() const { return down; }

    void
    setDown(bool state);

    Tool*
    tool() const { return instance; }

signals:
    void
    pressed();

protected:
    virtual
    void
    paintEvent(QPaintEvent *event);

    virtual
    void
    mousePressEvent(QMouseEvent *event);

    virtual
    bool
    event(QEvent *event);

protected slots:
    void
    select();

private slots:
    void
    animValueChanged(const QVariant &value);

private:
    Tool *instance;

    VariantAnimation *anim;

    bool down;
};

#endif // TOOLPANELBUTTON_H
