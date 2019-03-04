#ifndef PANELCOMBOBOX_H
#define PANELCOMBOBOX_H

#include <QtCore/QVariant>

#include <QtWidgets/QWidget>

class Model;
class PanelButton;
class PanelListBox;
class PopUpWindow;

class WeightSlider : public QWidget
{
    Q_OBJECT

public:
    WeightSlider(Model *model, QWidget *parent = 0);

    void
    setJoint(int index);

    int
    joint() const { return jointIndex; }

    void
    setValue(float value);

    float
    value() const { return val; }

signals:
    void
    changed(float value);

protected:
    virtual
    void
    paintEvent(QPaintEvent *event);

    virtual
    void
    mousePressEvent(QMouseEvent *event);

    virtual
    void
    mouseMoveEvent(QMouseEvent *event);

    virtual
    void
    mouseReleaseEvent(QMouseEvent *event);

private:
    void
    change(int x);

    float
    valueFromPos(int x) const;

    Model *model;
    int jointIndex;
    float val;
    float prev;
};

class WeightCombo : public QWidget
{
    Q_OBJECT

public:
    WeightCombo(Model *model, QWidget *parent = 0);

    int
    joint() const { return slider->joint(); }

    void
    setJoint(int index);

    float
    value() const { return slider->value(); }

    void
    setValue(float value);

    virtual
    QSize
    sizeHint() const;

signals:
    void
    changed();

private slots:
    void
    buttonPressed();

    void
    windowHidden();

    void
    rowClicked(int index);

    void
    sliderChanged(float value);

private:
    Model *model;

    WeightSlider *slider;
    PanelButton *button;
    PanelListBox *listBox;
    PopUpWindow *window;
};

#endif // PANELCOMBOBOX_H
