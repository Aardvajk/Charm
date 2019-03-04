#ifndef RADIOBOX_H
#define RADIOBOX_H

#include <QtCore/QVariant>

#include "gui/PanelGroup.h"

class PanelButton;

class RadioBox : public PanelGroup
{
    Q_OBJECT

public:
    RadioBox(QWidget *parent = 0, Qt::Orientation orientation = Qt::Horizontal);

    void
    setButtonMinimumSize(const QSize &size);

    void
    setButtonMaximumSize(const QSize &size);

    void
    addItem(const QString &text, const QVariant &data = QVariant());

    QVariant
    value() const;

    void
    setCurrentData(const QVariant &data);

signals:
    void
    valueChanged(const QVariant &data);

private slots:
    void
    buttonToggled(bool state);

private:
    QVector<PanelButton*> buttons;

    QSize buttonMinSize;
    QSize buttonMaxSize;
};

#endif // RADIOBOX_H
