#ifndef BACKGROUNDPANEL_H
#define BACKGROUNDPANEL_H

#include <QtWidgets/QWidget>

class QBoxLayout;

class BackgroundPanel : public QWidget
{
    Q_OBJECT

public:
    BackgroundPanel(QWidget *parent = 0);

    QBoxLayout*
    boxLayout() const;
};

#endif // BACKGROUNDPANEL_H
