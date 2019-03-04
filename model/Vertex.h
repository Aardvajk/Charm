#ifndef VERTEX_H
#define VERTEX_H

#include "maths/Vec3.h"

#include "common/StoredItem.h"
#include "common/Optional.h"

#include <QtCore/QDataStream>

class VertexInfluence
{
public:
    VertexInfluence() : joint(-1), weight(0) { }
    VertexInfluence(int joint, float weight) : joint(joint), weight(weight) { }

    bool
    operator==(const VertexInfluence &v) const { return joint == v.joint && qFuzzyCompare(weight, v.weight); }

    bool
    operator!=(const VertexInfluence &v) const { return joint != v.joint || !qFuzzyCompare(weight, v.weight); }

    int joint;
    float weight;
};

class Vertex
{
public:
    Vertex() : pos(0, 0, 0), selected(false), hide(0), group(0) { }
    Vertex(const Vec3 &pos, bool selected = false) : pos(pos), selected(selected), hide(0), group(0) { }

    static const int maximumInfluences = 4;

    Vec3 pos;
    bool selected;
    int hide;
    int group;
    optional<Vec3> manualNormal;

    VertexInfluence influences[maximumInfluences];
};

typedef StoredItem<Vertex> StoredVertex;

inline QDataStream &operator<<(QDataStream &stream, const Vertex &v)
{
    return stream << v.pos.x << v.pos.y << v.pos.z;
}

inline QDataStream &operator>>(QDataStream &stream, Vertex &v)
{
    return stream >> v.pos.x >> v.pos.y >> v.pos.z;
}

#endif // VERTEX_H
