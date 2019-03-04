#include "OptionsPanel.h"
#include "Separator.h"

#include "style/Style.h"

#include <QtWidgets/QVBoxLayout>
#include <QtGui/QPainter>

OptionsPanel::OptionsPanel(QWidget *parent)
    : QWidget(parent)
{
    layout = new QVBoxLayout(this);
    layout->setSpacing(10);

    setMaximumWidth(240);
    setMinimumWidth(240);

    setAutoFillBackground(true);

    QPalette pal = palette();
    pal.setColor(QPalette::Background, Style::baseColor().darker(140));

    setPalette(pal);
}

void
OptionsPanel::addWidget(QWidget *widget)
{
    layout->addWidget(widget);
}

QWidget*
OptionsPanel::addSeparator()
{
    QWidget *widget = new Separator();
    layout->addWidget(widget);

    return widget;
}

void
OptionsPanel::addStretch()
{
    layout->addStretch();
}
