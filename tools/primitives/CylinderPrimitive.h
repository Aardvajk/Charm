#ifndef CYLINDERPRIMITIVE_H
#define CYLINDERPRIMITIVE_H

#include "Primitive.h"

#include "maths/Vec3.h"

class CreateCommand;
class Settings;

class CylinderPrimitive : public Primitive
{
    Q_OBJECT

public:
    CylinderPrimitive(Settings *settings, Model *model, QObject *parent = 0);

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
    createCylinder(const Vec3 &pos, bool preserve);

    CreateCommand*
    makeCylinder(const Vec3 &pos, int segments, int slices, float radius, float height, bool cap);

    Property *segments;
    Property *slices;
    Property *radius;
    Property *height;
    Property *cap;
};

#endif // CYLINDERPRIMITIVE_H
