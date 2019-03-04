#ifndef ROWWIDGET_H
#define ROWWIDGET_H

#include <QtWidgets/QWidget>

class QBoxLayout;

class RowWidget : public QWidget
{
    Q_OBJECT

public:
    RowWidget(QWidget *parent = 0);

    void
    addWidget(QWidget *widget);

    void
    addStretch();

private:
    QBoxLayout *boxLayout;
};

#endif // ROWWIDGET_H
