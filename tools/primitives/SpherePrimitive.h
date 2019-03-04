#ifndef SPHEREPRIMITIVE_H
#define SPHEREPRIMITIVE_H

#include "Primitive.h"

#include "maths/Vec3.h"

class CreateCommand;
class Settings;

class SpherePrimitive : public Primitive
{
    Q_OBJECT

public:
    SpherePrimitive(Settings *settings, Model *model, QObject *parent = 0);

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
    createSphere(const Vec3 &pos, bool preserve);

    CreateCommand*
    makeSphere(const Vec3 &pos, int rings, int segments, float radius);

    Property *rings;
    Property *segments;
    Property *radius;
};

#endif // SPHEREPRIMITIVE_H
