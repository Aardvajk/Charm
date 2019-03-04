#ifndef LINEHANDLE_H
#define LINEHANDLE_H

#include <QtCore/QObject>
#include <QtCore/QPoint>

class ModelView;
class Graphics;

class LineHandle : public QObject
{
    Q_OBJECT

public:
    LineHandle(QObject *parent = 0);

    void
    setAnchor(const QPoint &p);

    void
    setPosition(const QPoint &p);

    ModelView*
    view() const { return activeView; }

    void
    setActiveView(ModelView *view);

public slots:
    virtual
    void
    render(ModelView *view, Graphics *graphics);

private:
    ModelView *activeView;

    QPoint anc;
    QPoint pos;
};

#endif // LINEHANDLE_H
