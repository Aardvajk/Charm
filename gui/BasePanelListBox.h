#ifndef BASEPANELLISTBOX_H
#define BASEPANELLISTBOX_H

#include <QtCore/QString>
#include <QtCore/QVariant>

#include <QtWidgets/QAbstractScrollArea>
#include <QtGui/QFont>

class PanelListBoxItem
{
public:
    PanelListBoxItem(const QString &text = QString(), const QVariant &data = QVariant()) : t(text), d(data), fValid(false) { }

    QString
    text() const { return t; }

    void
    setText(const QString &value){ t = value; }

    QFont
    font() const { return f; }

    bool
    fontValid() const { return fValid; }

    void
    setFont(const QFont &value){ f = value; fValid = true; }

    QColor
    color() const { return c; }

    void
    setColor(const QColor &value){ c = value; }

    QVariant
    data() const { return d; }

    void
    setData(const QVariant &value){ d = value; }

private:
    QString t;
    QVariant d;

    QFont f;
    bool fValid;

    QColor c;
};

class BasePanelListBox : public QAbstractScrollArea
{
    Q_OBJECT

public:
    BasePanelListBox(QWidget *parent = 0);

    void
    addItem(const QString &text, const QVariant &data = QVariant());

    int
    count() const { return items.count(); }

    QString
    itemText(int index) const { return items[index].text(); }

    void
    setItemText(int index, const QString &text);

    QVariant
    itemData(int index) const { return items[index].data(); }

    QFont
    itemFont(int index) const;

    void
    setItemFont(int index, const QFont &font);

    void
    setItemColor(int index, const QColor &color);

    void
    setItemData(int index, const QVariant &data);

    int
    heightForItems(int count) const;

    void
    ensureVisible(int index);

    int
    currentScroll() const;

    void
    setCurrentScroll(int value);

signals:
    void
    clicked(int row);

protected:
    virtual
    void
    paintEvent(QPaintEvent *event);

    virtual
    void
    resizeEvent(QResizeEvent *event);

    virtual
    bool
    drawItemSelected(int index) const = 0;

    virtual
    void
    paintItem(QPainter &painter, const QRect &rect, int index);

protected:
    void
    updateScroll();

    int
    itemHeight() const;

    QVector<PanelListBoxItem> items;
    int hover;
};

#endif // BASEPANELLISTBOX_H
