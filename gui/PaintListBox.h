#ifndef PAINTLISTBOX_H
#define PAINTLISTBOX_H

#include "PanelListBox.h"

class PaintListBox : public PanelListBox
{
    Q_OBJECT

public:
    PaintListBox(QWidget *parent = 0);

protected:
    virtual
    void
    paintItem(QPainter &painter, const QRect &rect, int index);
};

#endif // PAINTLISTBOX_H
