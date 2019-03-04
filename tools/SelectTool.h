#ifndef SELECTTOOL_H
#define SELECTTOOL_H

#include "tools/BaseModelViewTool.h"

#include "common/VariantSet.h"
#include "common/Mode.h"

#include <QtCore/QMap>

#include <memory>

class ActionList;
class BoxMarquee;
class ToggleBox;
class QCheckBox;

class SelectToolDetails
{
public:
    SelectToolDetails() : frontOnly(false) { }

    VariantSet stored;
    bool frontOnly;
};

class SelectTool : public BaseModelViewTool
{
    Q_OBJECT

public:
    SelectTool(Settings *settings, ActionList *actions, Model *model, ModelViewGrid *viewGrid, float jointRadius, QObject *parent = 0);

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

    ToggleBox*
    elementsBox() const { return elementBox; }

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
    elementsChanged(const QVariant &value, bool state);

    void
    frontOnlyToggled(bool state);

    void
    modeChanged();

private:
    BoxMarquee *marquee;
    Qt::MouseButton startButton;

    std::auto_ptr<QWidget> options;
    ToggleBox *elementBox;
    QCheckBox *frontOnlyCheck;

    QMap<Mode::Type, SelectToolDetails> details;

    float jointRadius;
};

#endif // SELECTTOOL_H
