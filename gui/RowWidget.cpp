#include "RowWidget.h"

#include <QtWidgets/QHBoxLayout>

RowWidget::RowWidget(QWidget *parent)
    : QWidget(parent)
{
    boxLayout = new QHBoxLayout(this);
    boxLayout->setMargin(0);
}

void
RowWidget::addWidget(QWidget *widget)
{
    boxLayout->addWidget(widget);
}

void
RowWidget::addStretch()
{
    boxLayout->addStretch();
}
