#include "PanelGroup.h"

#include "style/Style.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>

PanelGroup::PanelGroup(QWidget *parent, Qt::Orientation orientation)
{
    QBoxLayout *layout;

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    if(orientation == Qt::Horizontal)
    {
        layout = new QHBoxLayout(this);
    }
    else
    {
        layout = new QVBoxLayout(this);
    }

    layout->setMargin(1);
    layout->setSpacing(1);

    QPalette pal = palette();
    pal.setColor(QPalette::Background, Style::baseColor().darker(240));

    setPalette(pal);
    setAutoFillBackground(true);
}

void
PanelGroup::addWidget(QWidget *widget)
{
    layout()->addWidget(widget);
}
