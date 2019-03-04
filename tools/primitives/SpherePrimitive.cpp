#include "SpherePrimitive.h"

#include "settings/Settings.h"

#include "maths/Maths.h"

#include "model/Model.h"

#include "command/CreateCommand.h"

#include "views/ModelView.h"

#include "properties/Property.h"

#include <QtGui/QMouseEvent>

SpherePrimitive::SpherePrimitive(Settings *settings, Model *model, QObject *parent)
    : Primitive(model, parent)
{
    Settings *child = settings->child("Sphere");

    rings = new Property(Property::Int, "Rings", child->child("rings")->value(10), this);
    rings->setRange(3, QVariant());

    segments = new Property(Property::Int, "Segments", child->child("segments")->value(10), this);
    segments->setRange(3, QVariant());

    radius = new Property(Property::Float, "Radius", child->child("radius")->value(10), this);
    radius->setRange(0, QVariant());
}

QString
SpherePrimitive::name() const
{
    return "Sphere";
}

void
SpherePrimitive::create(const Vec3 &pos)
{
    createSphere(pos, false);
}

QVector<Property*>
SpherePrimitive::properties() const
{
    return QVector<Property*>() << rings << segments << radius;
}

void
SpherePrimitive::mousePressed(ModelView *view, QMouseEvent *event)
{
    createSphere(view->getWorldPosition(event->pos(), model()->referencePoint()), event->modifiers() & Qt::ShiftModifier);
}

void
SpherePrimitive::saveSettings(Settings *settings) const
{
    Settings *child = settings->child("Sphere");
    child->clear();

    child->add("rings", rings->value());
    child->add("segments", segments->value());
    child->add("radius", radius->value());
}

void
SpherePrimitive::createSphere(const Vec3 &pos, bool preserve)
{
    CreateCommand *command = makeSphere(pos, rings->value().toInt(), segments->value().toInt(), radius->value().toFloat());
    command->activate(preserve);

    model()->endCommand(command);
}

CreateCommand*
SpherePrimitive::makeSphere(const Vec3 &pos, int rings, int segments, float radius)
{
    QVector<Vertex> verts;
    QVector<Face> faces;

    const float twoPi = pi() * 2;
    const float ydiff = 1.0f / (rings - 1.0f);
    const float xdiff = twoPi / segments;

    float y = 0;

    int start = model()->vertexCount();

    for(int ring = 0; ring < rings; ++ring)
    {
        if(ring == 0)
        {
            verts.append(Vertex(Vec3(0, radius, 0) + pos, true));
        }
        else if(ring == rings - 1)
        {
            verts.append(Vertex(Vec3(0, -radius, 0) + pos, true));
        }
        else
        {
            float xdelta = sinf(y * pi()) * radius;
            float ydelta = cosf(y * pi()) * radius;

            float a = 0;
            for(int x = 0; x < segments; ++x)
            {
                verts.append(Vertex(Vec3(-sinf(a) * xdelta, ydelta, cosf(a) * xdelta) + pos, true));

                a += xdiff;
            }
        }

        y += ydiff;
    }

    for(int ring = 0; ring < rings - 1; ++ring)
    {
        if(ring == 0)
        {
            for(int i = 0; i < segments; ++i)
            {
                int j = (i < segments - 1 ? i + 1 : 0);

                Face face(start, start + 1 + j, start + 1 + i);
                face.selected = true;

                faces.append(face);
            }
        }
        else if(ring == rings - 2)
        {
            int base = verts.count() - 1;

            for(int i = 0; i < segments; ++i)
            {
                int j = (i < segments - 1 ? i + 1 : 0);

                Face face(start + base - segments + i, start + base - segments + j, start + base);
                face.selected = true;

                faces.append(face);
            }
        }
        else
        {
            int vertex = 1 + ((ring - 1) * segments);

            for(int i = 0; i < segments; ++i)
            {
                int j = (i < segments - 1 ? i + 1 : 0);

                Face face(start + vertex + i, start + vertex + j, start + vertex + j + segments);
                face.selected = true;

                faces.append(face);

                face = Face(start + vertex + i, start + vertex + j + segments, start + vertex + i + segments);
                face.selected = true;

                faces.append(face);
            }
        }
    }

    CreateCommand *command = new CreateCommand("Add Sphere", model(), this);

    command->setVertices(verts);
    command->setFaces(faces);

    return command;
}
