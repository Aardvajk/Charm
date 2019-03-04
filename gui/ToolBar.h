#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QtWidgets/QWidget>

class QHBoxLayout;

class ToolBar : public QWidget
{
    Q_OBJECT

public:
    ToolBar(QWidget *parent = 0);

    void
    addWidget(QWidget *widget);

    void
    addStretch();

signals:
    void
    doubleClicked();

protected:
    virtual
    void
    paintEvent(QPaintEvent *event);

    virtual
    void
    mouseDoubleClickEvent(QMouseEvent *event);

private:
    QHBoxLayout *layout;
};

#endif // TOOLBAR_H
