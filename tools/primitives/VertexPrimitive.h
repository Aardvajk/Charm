#ifndef VERTEXPRIMITIVE_H
#define VERTEXPRIMITIVE_H

#include "Primitive.h"

#include "maths/Vec3.h"

class VertexPrimitive : public Primitive
{
    Q_OBJECT

public:
    VertexPrimitive(Model *model, QObject *parent = 0);

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
    createVertex(const Vec3 &pos, bool preserve);
};

#endif // VERTEXPRIMITIVE_H
