#ifndef GRAPHICSWIDGET_H
#define GRAPHICSWIDGET_H

#include <QtWidgets/QWidget>

class GraphicsDevice;

class GraphicsWidget : public QWidget
{
    Q_OBJECT

public:
    GraphicsWidget(GraphicsDevice *graphics, QWidget *parent = 0);
    ~GraphicsWidget();

protected:
    virtual
    QPaintEngine*
    paintEngine() const { return 0; }

    virtual
    void
    paintEvent(QPaintEvent *event);

    GraphicsDevice*
    graphicsDevice() const { return graphics; }

private:
    GraphicsDevice *graphics;
};

#endif // GRAPHICSWIDGET_H
