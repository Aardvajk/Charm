#include "BackgroundPanel.h"

#include "style/Style.h"

#include <QtWidgets/QHBoxLayout>

BackgroundPanel::BackgroundPanel(QWidget *parent)
    : QWidget(parent)
{
    setContentsMargins(1, 0, 1, 1);

    QPalette pal = palette();
    pal.setColor(QPalette::Background, Style::baseColor().darker(340));

    setPalette(pal);
    setAutoFillBackground(true);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(1);
}

QBoxLayout*
BackgroundPanel::boxLayout() const
{
    return static_cast<QBoxLayout*>(layout());
}
