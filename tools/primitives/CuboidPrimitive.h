#ifndef CUBOIDPRIMITIVE_H
#define CUBOIDPRIMITIVE_H

#include "Primitive.h"

#include "maths/Vec3.h"

class CreateCommand;
class Settings;

class CuboidPrimitive : public Primitive
{
    Q_OBJECT

public:
    CuboidPrimitive(Settings *settings, Model *model, QObject *parent = 0);

    virtual
    QString
    name() const;

    virtual
    void
    create(const Vec3 &pos);

    QVector<Property*>
    properties() const;

    virtual
    void
    mousePressed(ModelView *view, QMouseEvent *event);

    virtual
    void
    saveSettings(Settings *settings) const;

private:
    void
    createCuboid(const Vec3 &pos, bool preserve);

    CreateCommand*
    makeCuboid(const Vec3 &pos, float width, float height, float depth);

    Property *width;
    Property *height;
    Property *depth;
};

#endif // CUBOIDPRIMITIVE_H
