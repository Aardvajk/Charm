#include "CylinderPrimitive.h"

#include "settings/Settings.h"

#include "maths/Maths.h"

#include "model/Model.h"

#include "command/CreateCommand.h"

#include "views/ModelView.h"

#include "properties/Property.h"

#include <QtGui/QMouseEvent>

CylinderPrimitive::CylinderPrimitive(Settings *settings, Model *model, QObject *parent)
    : Primitive(model, parent)
{
    Settings *child = settings->child("Cylinder");

    segments = new Property(Property::Int, "Segments", child->child("segments")->value(10), this);
    segments->setRange(3, QVariant());

    slices = new Property(Property::Int, "Slices", child->child("slices")->value(4), this);
    slices->setRange(1, QVariant());

    radius = new Property(Property::Float, "Radius", child->child("radius")->value(10), this);
    radius->setRange(0, QVariant());

    height = new Property(Property::Float, "Height", child->child("height")->value(20), this);
    height->setRange(0, QVariant());

    cap = new Property(Property::Bool, "Cap Ends", child->child("cap")->value(true), this);
}

QString
CylinderPrimitive::name() const
{
    return "Cylinder";
}

void
CylinderPrimitive::create(const Vec3 &pos)
{
    createCylinder(pos, false);
}

QVector<Property*>
CylinderPrimitive::properties() const
{
    return QVector<Property*>() << segments << slices << radius << height << cap;
}

void
CylinderPrimitive::mousePressed(ModelView *view, QMouseEvent *event)
{
    createCylinder(view->getWorldPosition(event->pos(), model()->referencePoint()), event->modifiers() & Qt::ShiftModifier);
}

void
CylinderPrimitive::saveSettings(Settings *settings) const
{
    Settings *child = settings->child("Cylinder");
    child->clear();

    child->add("segments", segments->value());
    child->add("slices", slices->value());
    child->add("radius", radius->value());
    child->add("height", height->value());
    child->add("cap", cap->value());
}

void
CylinderPrimitive::createCylinder(const Vec3 &pos, bool preserve)
{
    CreateCommand *command = makeCylinder(pos, segments->value().toInt(), slices->value().toInt(), radius->value().toFloat(), height->value().toFloat(), cap->value().toBool());
    command->activate(preserve);

    model()->endCommand(command);
}

CreateCommand*
CylinderPrimitive::makeCylinder(const Vec3 &pos, int segments, int slices, float radius, float height, bool cap)
{
    QVector<Vertex> verts;
    QVector<Face> faces;

    int start = model()->vertexCount();

    int rings = slices + 1;

    const float twoPi = pi() * 2;
    const float xdiff = twoPi / segments;
    const float ydiff = height / (rings - 1.0f);
    const float half = height / 2;

    float y = half;

    for(int ring = 0; ring < rings; ++ring)
    {
        float a = 0;
        for(int x = 0; x < segments; ++x)
        {
            verts.append(Vertex(Vec3(-sinf(a) * radius, y, cosf(a) * radius) + pos, true));

            a += xdiff;
        }

        y -=ydiff;
    }

    for(int ring = 1; ring < rings; ++ring)
    {
        int vertex = (ring - 1) * segments;

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

    if(cap)
    {
        int base = verts.count();
        int last = verts.count() - segments;

        verts.append(Vertex(Vec3(0, half, 0) + pos, true));

        for(int i = 0; i < segments; ++i)
        {
            verts.append(verts[i]);

            int j = (i < segments - 1 ? i + 1 : 0);

            Face face(start + base, start + base + 1 + j, start + base + 1 + i);
            face.selected = true;

            faces.append(face);
        }

        base = verts.count();

        verts.append(Vertex(Vec3(0, -half, 0) + pos, true));

        for(int i = 0; i < segments; ++i)
        {
            verts.append(verts[last + i]);

            int j = (i < segments - 1 ? i + 1 : 0);

            Face face(start + base, start + base + 1 + i, start + base + 1 + j);
            face.selected = true;

            faces.append(face);
        }
    }

    CreateCommand *command = new CreateCommand("Add Cylinder", model(), this);

    command->setVertices(verts);
    command->setFaces(faces);

    return command;
}
