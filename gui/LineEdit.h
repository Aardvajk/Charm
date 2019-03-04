#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QtWidgets/QLineEdit>

class VariantAnimation;

class LineEdit : public QLineEdit
{
    Q_OBJECT

public:
    LineEdit(QWidget *parent = 0);

    void
    setHintText(const QString &text);

protected:
    virtual
    void
    paintEvent(QPaintEvent *event);

    virtual
    void
    mousePressEvent(QMouseEvent *event);

    void
    mouseReleaseEvent(QMouseEvent *event);

    virtual
    void
    mouseMoveEvent(QMouseEvent *event);

    void
    updateText(const QString &text);

private slots:
    void
    deleteAnimChanged(const QVariant &value);

    void
    textChangedHandler(const QString &text);

private:
    QRect
    iconRect() const;

    VariantAnimation *deleteButtonAnim;
    float deleteAlpha;

    bool mousePressOverDeleteIcon;

    QString hintText;
};

#endif // LINEEDIT_H
