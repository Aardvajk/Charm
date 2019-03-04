#ifndef JOINTPRIMITIVE_H
#define JOINTPRIMITIVE_H

#include "Primitive.h"

class JointPrimitive : public Primitive
{
    Q_OBJECT

public:
    JointPrimitive(Model *model, QObject *parent = 0);

    virtual
    QString
    name() const;

    virtual
    void
    create(const Vec3 &pos);

    virtual
    void
    mousePressed(ModelView *view, QMouseEvent *event);

private:
    void
    createJoint(const Vec3 &pos, bool preserve);
};

#endif // JOINTPRIMITIVE_H
