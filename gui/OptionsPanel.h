#ifndef OPTIONSPANEL_H
#define OPTIONSPANEL_H

#include <QtWidgets/QWidget>

class QBoxLayout;

class OptionsPanel : public QWidget
{
    Q_OBJECT

public:
    OptionsPanel(QWidget *parent = 0);

    void
    addWidget(QWidget *widget);

    QWidget*
    addSeparator();

    void
    addStretch();

private:
    QBoxLayout *layout;
};

#endif // OPTIONSPANEL_H
