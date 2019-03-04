#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <QtCore/QVector>
#include <QtCore/QObject>

#include "maths/Vec3.h"

class Model;
class ModelView;
class Property;
class Settings;
class QMouseEvent;

class Primitive : public QObject
{
    Q_OBJECT

public:
    Primitive(Model *model, QObject *parent = 0);

    virtual
    QString
    name() const = 0;

    virtual
    QVector<Property*>
    properties() const;

    virtual
    void
    create(const Vec3 &pos) = 0;

    virtual
    void
    mousePressed(ModelView *view, QMouseEvent *event);

    virtual
    void
    mouseMoved(ModelView *view, QMouseEvent *event);

    virtual
    void
    mouseReleased(ModelView *view, QMouseEvent *event);

    virtual
    void
    focusLost();

    virtual
    void
    saveSettings(Settings *settings) const;

protected:
    Model*
    model() const { return currentModel; }

private:
    Model *currentModel;
};

#endif // PRIMITIVE_H
