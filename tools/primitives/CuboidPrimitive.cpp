#include "CuboidPrimitive.h"

#include "settings/Settings.h"

#include "maths/Maths.h"

#include "model/Model.h"

#include "command/CreateCommand.h"

#include "views/ModelView.h"

#include "properties/Property.h"

#include <QtGui/QMouseEvent>

namespace
{

Face
makeFace(int a, int b, int c)
{
    Face face(a, b, c);
    face.selected = true;

    return face;
}

}

CuboidPrimitive::CuboidPrimitive(Settings *settings, Model *model, QObject *parent)
    : Primitive(model, parent)
{
    Settings *child = settings->child("Cuboid");

    width = new Property(Property::Float, "Width", child->child("width")->value(20), this);
    width->setRange(0, QVariant());

    height = new Property(Property::Float, "Height", child->child("height")->value(20), this);
    height->setRange(0, QVariant());

    depth = new Property(Property::Float, "Depth", child->child("depth")->value(20), this);
    depth->setRange(0, QVariant());
}

QString
CuboidPrimitive::name() const
{
    return "Cuboid";
}

void
CuboidPrimitive::create(const Vec3 &pos)
{
    createCuboid(pos, false);
}

QVector<Property*>
CuboidPrimitive::properties() const
{
    return QVector<Property*>() << width << height << depth;
}

void
CuboidPrimitive::mousePressed(ModelView *view, QMouseEvent *event)
{
    createCuboid(view->getWorldPosition(event->pos(), model()->referencePoint()), event->modifiers() & Qt::ShiftModifier);
}

void
CuboidPrimitive::saveSettings(Settings *settings) const
{
    Settings *child = settings->child("Cuboid");
    child->clear();

    child->add("width", width->value());
    child->add("height", height->value());
    child->add("depth", depth->value());
}

void
CuboidPrimitive::createCuboid(const Vec3 &pos, bool preserve)
{
    CreateCommand *command = makeCuboid(pos, width->value().toFloat(), height->value().toFloat(), depth->value().toFloat());
    command->activate(preserve);

    model()->endCommand(command);
}

CreateCommand*
CuboidPrimitive::makeCuboid(const Vec3 &pos, float width, float height, float depth)
{
    QVector<Vertex> verts;
    QVector<Face> faces;

    const float w = width / 2;
    const float h = height / 2;
    const float d = depth / 2;

    int base = model()->vertexCount();

    verts.append(Vertex(Vec3(-w, h, -d) + pos, true));
    verts.append(Vertex(Vec3(w, h, -d) + pos, true));
    verts.append(Vertex(Vec3(w, -h, -d) + pos, true));
    verts.append(Vertex(Vec3(-w, -h, -d) + pos, true));

    faces.append(makeFace(base, base + 1, base + 2));
    faces.append(makeFace(base, base + 2, base + 3));

    base += 4;

    verts.append(Vertex(Vec3(-w, h, d) + pos, true));
    verts.append(Vertex(Vec3(w, h, d) + pos, true));
    verts.append(Vertex(Vec3(w, -h, d) + pos, true));
    verts.append(Vertex(Vec3(-w, -h, d) + pos, true));

    faces.append(makeFace(base, base + 2, base + 1));
    faces.append(makeFace(base, base + 3, base + 2));

    base += 4;

    verts.append(Vertex(Vec3(-w, h, d) + pos, true));
    verts.append(Vertex(Vec3(-w, h, -d) + pos, true));
    verts.append(Vertex(Vec3(-w, -h, -d) + pos, true));
    verts.append(Vertex(Vec3(-w, -h, d) + pos, true));

    faces.append(makeFace(base, base + 1, base + 2));
    faces.append(makeFace(base, base + 2, base + 3));

    base += 4;

    verts.append(Vertex(Vec3(w, h, d) + pos, true));
    verts.append(Vertex(Vec3(w, h, -d) + pos, true));
    verts.append(Vertex(Vec3(w, -h, -d) + pos, true));
    verts.append(Vertex(Vec3(w, -h, d) + pos, true));

    faces.append(makeFace(base, base + 2, base + 1));
    faces.append(makeFace(base, base + 3, base + 2));

    base += 4;

    verts.append(Vertex(Vec3(-w, h, d) + pos, true));
    verts.append(Vertex(Vec3(w, h, d) + pos, true));
    verts.append(Vertex(Vec3(w, h, -d) + pos, true));
    verts.append(Vertex(Vec3(-w, h, -d) + pos, true));

    faces.append(makeFace(base, base + 1, base + 2));
    faces.append(makeFace(base, base + 2, base + 3));

    base += 4;

    verts.append(Vertex(Vec3(-w, -h, d) + pos, true));
    verts.append(Vertex(Vec3(w, -h, d) + pos, true));
    verts.append(Vertex(Vec3(w, -h, -d) + pos, true));
    verts.append(Vertex(Vec3(-w, -h, -d) + pos, true));

    faces.append(makeFace(base, base + 2, base + 1));
    faces.append(makeFace(base, base + 3, base + 2));

    CreateCommand *command = new CreateCommand("Add Cuboid", model(), this);

    command->setVertices(verts);
    command->setFaces(faces);

    return command;
}

