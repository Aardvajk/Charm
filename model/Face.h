#ifndef FACE_H
#define FACE_H

#include "common/StoredItem.h"

#include "maths/Maths.h"

#include <QtCore/QDataStream>

class Face
{
public:
    Face() : selected(false), hide(0), group(0), palette(-1) { indices[0] = indices[1] = indices[2] = -1; }
    Face(int a, int b, int c) : selected(false), hide(0), group(0), palette(-1) { indices[0] = a; indices[1] = b; indices[2] = c; }

    bool
    contains(int vertex) const { return indices[0] == vertex || indices[1] == vertex || indices[2] == vertex; }

    void
    replace(int a, int b){ for(int i = 0; i < 3; ++i){ if(indices[i] == a) indices[i] = b; } }

    int indices[3];
    bool selected;
    int hide;
    int group;
    int palette;
};

typedef StoredItem<Face> StoredFace;

inline QDataStream &operator<<(QDataStream &stream, const Face &f)
{
    return stream << f.indices[0] << f.indices[1] << f.indices[2];
}

inline QDataStream &operator>>(QDataStream &stream, Face &f)
{
    return stream >> f.indices[0] >> f.indices[1] >> f.indices[2];
}

#endif // FACE_H
