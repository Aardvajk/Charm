#ifndef BOXMARQUEE_H
#define BOXMARQUEE_H

#include <QtCore/QObject>
#include <QtCore/QPoint>
#include <QtCore/QRectF>

class ModelView;
class Graphics;

class BoxMarquee : public QObject
{
    Q_OBJECT

public:
    BoxMarquee(QObject *parent = 0);

    void
    setAnchor(const QPoint &p);

    void
    setPosition(const QPoint &p);

    ModelView*
    view() const { return activeView; }

    void
    setActiveView(ModelView *view);

    QRectF
    clipRect(const QSize &size) const;

public slots:
    virtual
    void
    render(ModelView *view, Graphics *graphics);

private:
    ModelView *activeView;

    QPoint anc;
    QPoint pos;
};

#endif // BOXMARQUEE_H
