#ifndef ANCHORWIDGET_H
#define ANCHORWIDGET_H

#include <QtWidgets/QWidget>

#include "common/Anchor.h"

class ActionList;
class Model;
class RadioBox;
class QVariant;

class AnchorWidget : public QWidget
{
    Q_OBJECT

public:
    AnchorWidget(ActionList *actions, Model *model, QWidget *parent = 0);

    Anchor::Type
    type() const;

public slots:
    void
    setType(Anchor::Type type);

private slots:
    void
    anchorChanged(const QVariant &value);

    void
    modelAnchorChanged();

    void
    actionTriggered();

private:
    void
    createAction(ActionList *actions, const QString &name, const QString &key, int data);

    Model *model;
    RadioBox *box;
};

#endif // ANCHORWIDGET_H
