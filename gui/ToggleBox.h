#ifndef TOGGLEBOX_H
#define TOGGLEBOX_H

#include <QtCore/QVariant>

#include "common/VariantSet.h"

#include "gui/PanelGroup.h"

class PanelButton;

class ToggleBox : public PanelGroup
{
    Q_OBJECT

public:
    ToggleBox(QWidget *parent = 0);

    void
    addItem(const QString &text, const QVariant &data = QVariant());

    VariantSet
    values() const;

    bool
    value(const QVariant &item) const;

    void
    setValue(const QVariant &value, bool state);

    void
    setValues(const VariantSet &values);

    void
    setEnabledValues(const VariantSet &values);

signals:
    void
    changed(const QVariant &value, bool state);

private slots:
    void
    buttonToggled(bool state);

private:
    QVector<PanelButton*> buttons;
};

#endif // TOGGLEBOX_H
