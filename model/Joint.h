#ifndef JOINT_H
#define JOINT_H

#include "maths/Vec3.h"
#include "maths/Quaternion.h"

#include "common/StoredItem.h"

#include <QtCore/QDataStream>
#include <QtCore/QString>

#include <QtGui/QColor>

class JointTransform
{
public:
    JointTransform() : rotation(0, 0, 0, 1), translation(0, 0, 0) { }

    Quaternion rotation;
    Vec3 translation;
};

JointTransform
interpolateJointTransforms(const JointTransform &a, const JointTransform &b, float t);

class JointDetails
{
public:
    JointDetails(const QString &name = "New Joint") : name(name), parent(-1), paired(-1) { }

    QString name;
    int parent;
    int paired;
};

class Joint
{
public:
    Joint() : pos(0, 0, 0), selected(false), hide(0), group(0) { }
    Joint(const Vec3 &pos, bool selected = false) : pos(pos), selected(selected), hide(0), group(0) { }

    static const int maximumJoints = 59;

    static
    QColor
    color(){ return Qt::green; }

    Vec3 pos;
    bool selected;
    int hide;
    int group;

    JointTransform transform;
    JointDetails details;
};

typedef StoredItem<Joint> StoredJoint;

inline QDataStream &operator<<(QDataStream &stream, const Joint &j)
{
    return stream << j.pos.x << j.pos.y << j.pos.z << j.details.parent;
}

inline QDataStream &operator>>(QDataStream &stream, Joint &j)
{
    return stream >> j.pos.x >> j.pos.y >> j.pos.z >> j.details.parent;
}

#endif // JOINT_H
