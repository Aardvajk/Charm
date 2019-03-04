#ifndef PANELSEPARATOR_H
#define PANELSEPARATOR_H

#include <QtWidgets/QWidget>

class PanelSeparator : public QWidget
{
    Q_OBJECT

public:
    PanelSeparator(QWidget *parent = 0);

    QSize
    sizeHint() const;

protected:
    virtual
    void
    paintEvent(QPaintEvent *event);
};

#endif // PANELSEPARATOR_H
