#ifndef PANELGROUP_H
#define PANELGROUP_H

#include <QtWidgets/QWidget>

class PanelGroup : public QWidget
{
    Q_OBJECT

public:
    PanelGroup(QWidget *parent = 0, Qt::Orientation orientation = Qt::Horizontal);

    void
    addWidget(QWidget *widget);
};

#endif // PANELGROUP_H
