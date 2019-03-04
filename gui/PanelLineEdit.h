#ifndef PANELLINEEDIT_H
#define PANELLINEEDIT_H

#include <QtWidgets/QLineEdit>

class PanelLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    PanelLineEdit(QWidget *parent = 0);

    void
    setFrame(bool state);

protected:
    virtual
    void
    paintEvent(QPaintEvent *event);

    virtual
    void
    focusInEvent(QFocusEvent *event);

    virtual
    void
    focusOutEvent(QFocusEvent *event);

    virtual
    void
    keyPressEvent(QKeyEvent *event);

    virtual
    bool
    eventFilter(QObject *object, QEvent *event);

private:
    void
    loseFocus();

    QColor baseColor;
    QColor disabledColor;

    bool frame;
};

#endif // PANELLINEEDIT_H
