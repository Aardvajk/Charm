#ifndef AXISWIDGET_H
#define AXISWIDGET_H

#include <QtWidgets/QWidget>

#include "maths/Vec3.h"

class QVBoxLayout;
class PanelGroup;
class PanelButton;
class PanelLineEdit;
class QHBoxLayout;
class PanelComboBox;
class Model;
class Settings;

class AxisWidget : public QWidget
{
    Q_OBJECT

public:
    AxisWidget(Model *model, const QString &actionText, bool transformOptions, bool cursorIcon, QWidget *parent = 0);

    enum TransformType { Absolute, Relative };

    Vec3
    values(const Vec3 &disabledValues = Vec3(0, 0, 0)) const;

    Vec3
    filter() const;

    TransformType
    transformType() const;

    void
    addWidget(QWidget *widget);

    void
    setPlaceholder(int value);

    void
    loadSettings(Settings *settings);

    void
    saveSettings(Settings *settings) const;

public slots:
    void
    enableAxis(int axis, bool state);

signals:
    void
    enabled(int axis, bool state);

    void
    actionClicked();

private slots:
    void
    axisEnabled(bool state);

private:
    void
    addRow(QVBoxLayout *layout, const QString &text, int index);

    Model *model;

    QVector<PanelButton*> buttons;
    QVector<PanelLineEdit*> edits;

    PanelGroup *actionRow;
    PanelButton *actionButton;
    PanelComboBox *optionsCombo;

    QHBoxLayout *buttonLayout;
};

#endif // AXISWIDGET_H
