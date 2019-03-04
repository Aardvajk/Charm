#ifndef PANELLISTBOX_H
#define PANELLISTBOX_H

#include "BasePanelListBox.h"

class PanelListBox : public BasePanelListBox
{
    Q_OBJECT

public:
    PanelListBox(QWidget *parent = 0);

    void
    clear();

    void
    setDrawSelection(bool state);

    void
    removeItem(int index);

    QVariant
    currentData() const;

    void
    setCurrentItem(int index);

    int
    currentItem() const { return current; }

signals:
    void
    currentChanged(int index);

    void
    itemDoubleClicked(int item);

protected:
    virtual
    void
    mouseDoubleClickEvent(QMouseEvent *event);

    virtual
    void
    mouseMoveEvent(QMouseEvent *event);

    virtual
    void
    mouseReleaseEvent(QMouseEvent *event);

    virtual
    bool
    event(QEvent *event);

    virtual
    bool
    drawItemSelected(int index) const;

private:
    int current;
    bool drawSelection;
};

#endif // PANELLISTBOX_H
