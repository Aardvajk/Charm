#ifndef PANELMULTILISTBOX_H
#define PANELMULTILISTBOX_H

#include "BasePanelListBox.h"

class PanelMultiListBox : public BasePanelListBox
{
    Q_OBJECT

public:
    PanelMultiListBox(QWidget *parent = 0);

    void
    clear();

    void
    setCurrentItems(const QVector<int> &items);

    QVector<int>
    currentItems() const { return current; }

signals:
    void
    currentChanged(const QVector<int> &items);

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
    QVector<int> current;
};

#endif // PANELMULTILISTBOX_H
