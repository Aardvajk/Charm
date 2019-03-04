#ifndef VIEWSETTINGSWIDGET_H
#define VIEWSETTINGSWIDGET_H

#include <QtWidgets/QAbstractScrollArea>

class ModelView;
class PanelComboBox;
class QCheckBox;

class ViewSettingsWidget : public QAbstractScrollArea
{
    Q_OBJECT

public:
    ViewSettingsWidget(QWidget *parent = 0);

    QSize
    sizeHint() const;

    void
    setModelView(ModelView *view);

private slots:
    void
    shadingChanged(int index);

    void
    wireframeToggled(bool state);

    void
    projectionChanged(int index);

    void
    jointsToggled(bool state);

    void
    paintChanged(int index);

    void
    reflectChanged(int index);

    void
    smoothToggled(bool state);

    void
    resetClicked();

private:
    ModelView *modelView;

    PanelComboBox *shadeCombo;
    QCheckBox *wireframeCheck;
    PanelComboBox *projectionCombo;

    QCheckBox *jointsCheck;
    PanelComboBox *paintCombo;

    PanelComboBox *reflectCombo;
    QCheckBox *smoothCheck;
};

#endif // VIEWSETTINGSWIDGET_H
