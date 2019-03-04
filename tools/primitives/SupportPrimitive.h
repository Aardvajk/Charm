#ifndef SUPPORTPRIMITIVE_H
#define SUPPORTPRIMITIVE_H

#include "Primitive.h"

#include "maths/Vec3.h"

class SupportPrimitive : public Primitive
{
    Q_OBJECT

public:
    SupportPrimitive(Model *model, QObject *parent = 0);

    virtual
    QString
    name() const;

    void
    create(const Vec3 &pos);

    virtual
    void
    mousePressed(ModelView *view, QMouseEvent *event);

private:
    void
    createShape(const Vec3 &pos, bool preserve);
};

#endif // SUPPORTPRIMITIVE_H
