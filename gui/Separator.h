#ifndef SEPARATOR_H
#define SEPARATOR_H

#include <QtWidgets/QWidget>

class Separator : public QWidget
{
public:
    Separator(QWidget *parent = 0);

protected:
    virtual
    void
    paintEvent(QPaintEvent *event);
};

#endif // SEPARATOR_H
