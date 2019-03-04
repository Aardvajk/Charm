#include "SelectCommand.h"

#include <QtCore/QRectF>
#include <QtGui/QPolygonF>

#include "debug/Assert.h"

#include "model/Model.h"
#include "model/ModelData.h"

#include "common/ModelElement.h"

#include "maths/Maths.h"
#include "maths/Matrix.h"

SelectCommand::SelectCommand(Model *model, QObject *parent)
    : Command(model, parent)
{
}

QString
SelectCommand::name() const
{
    return "Select";
}

bool
SelectCommand::isValid() const
{
    return verts.count() || faces.count() || joints.count();
}

bool
SelectCommand::doesModifyPersistentState() const
{
    return false;
}

void
SelectCommand::undo()
{
    for(int i = 0; i < verts.count(); ++i)
    {
        Assert(verts[i].first >= 0 && verts[i].first < modelData()->vertices.count());

        modelData()->vertices[verts[i].first].selected = !verts[i].second;
    }

    for(int i = 0; i < faces.count(); ++i)
    {
        Assert(faces[i].first >= 0 && faces[i].first < modelData()->faces.count());

        modelData()->faces[faces[i].first].selected = !faces[i].second;
    }

    for(int i = 0; i < joints.count(); ++i)
    {
        Assert(joints[i].first >= 0 && joints[i].first < modelData()->joints.count());

        modelData()->joints[joints[i].first].selected = !joints[i].second;
    }

    modelData()->model()->changeSelection();
}

void
SelectCommand::redo()
{
    for(int i = 0; i < verts.count(); ++i)
    {
        Assert(verts[i].first >= 0 && verts[i].first < modelData()->vertices.count());

        modelData()->vertices[verts[i].first].selected = verts[i].second;
    }

    for(int i = 0; i < faces.count(); ++i)
    {
        Assert(faces[i].first >= 0 && faces[i].first < modelData()->faces.count());

        modelData()->faces[faces[i].first].selected = faces[i].second;
    }

    for(int i = 0; i < joints.count(); ++i)
    {
        Assert(joints[i].first >= 0 && joints[i].first < modelData()->joints.count());

        modelData()->joints[joints[i].first].selected = joints[i].second;
    }

    modelData()->model()->changeSelection();
}

void
SelectCommand::select(const QRectF &rect, const Matrix &worldMatrix, const Matrix &transform, bool preserve, const VariantSet &elements, bool frontOnly, float jointRadius)
{
    bool any = false;

    if(!preserve)
    {
        foreach(int i, modelData()->model()->selectedVertices())
        {
            Vec3 v;
            if(modelData()->model()->mode() == Mode::Model)
            {
                v = transformCoord(modelData()->vertices[i].pos, transform);
            }
            else
            {
                v = transformCoord(modelData()->model()->transformedVertexPosition(i), transform);
            }

            if(!rect.contains(QPointF(v.x, -v.y)) || !elements.contains(ModelElement::Vertex))
            {
                modelData()->vertices[i].selected = false;
                verts.append(qMakePair(i, false));

                any = true;
            }
        }

        foreach(int i, modelData()->model()->selectedFaces())
        {
            bool intersects = elements.contains(ModelElement::Face) && triangleIntersects(i, rect, transform, frontOnly);

            if(!intersects)
            {
                modelData()->faces[i].selected = false;
                faces.append(qMakePair(i, false));

                any = true;
            }
        }

        foreach(int i, modelData()->model()->selectedJoints())
        {
            bool intersects = elements.contains(ModelElement::Joint) && jointIntersects(i, jointRadius, rect, worldMatrix, transform);

            if(!intersects)
            {
                modelData()->joints[i].selected = false;
                joints.append(qMakePair(i, false));

                any = true;
            }
        }
    }

    if(elements.contains(ModelElement::Vertex))
    {
        for(int i = 0; i < modelData()->vertices.count(); ++i)
        {
            bool state = modelData()->vertices[i].selected;

            Vec3 v;
            if(modelData()->model()->mode() == Mode::Model)
            {
                v = transformCoord(modelData()->vertices[i].pos, transform);
            }
            else
            {
                v = transformCoord(modelData()->model()->transformedVertexPosition(i), transform);
            }

            if(rect.contains(QPointF(v.x, -v.y)))
            {
                if(!state && !modelData()->vertices[i].hide)
                {
                    modelData()->vertices[i].selected = true;
                    verts.append(qMakePair(i, true));

                    any = true;
                }
            }
        }
    }

    if(elements.contains(ModelElement::Face))
    {
        for(int i = 0; i < modelData()->faces.count(); ++i)
        {
            bool state = modelData()->faces[i].selected;

            bool intersects = triangleIntersects(i, rect, transform, frontOnly);

            if(!state && intersects && !modelData()->faces[i].hide)
            {
                modelData()->faces[i].selected = true;
                faces.append(qMakePair(i, true));

                any = true;
            }
        }
    }

    if(elements.contains(ModelElement::Joint))
    {
        for(int i = 0; i < modelData()->joints.count(); ++i)
        {
            bool state = modelData()->joints[i].selected;

            bool intersects = jointIntersects(i, jointRadius, rect, worldMatrix, transform);

            if(!state && intersects && !modelData()->joints[i].hide)
            {
                modelData()->joints[i].selected = true;
                joints.append(qMakePair(i, true));

                any = true;
            }
        }
    }

    if(any)
    {
        modelData()->model()->changeSelection();
    }
}

void
SelectCommand::deselect(const QRectF &rect, const Matrix &worldMatrix, const Matrix &transform, const VariantSet &elements, bool frontOnly, float jointRadius)
{
    bool any = false;

    if(elements.contains(ModelElement::Vertex))
    {
        for(int i = 0; i < modelData()->vertices.count(); ++i)
        {
            bool state = modelData()->vertices[i].selected;
            Vec3 v = transformCoord(modelData()->vertices[i].pos, transform);

            if(rect.contains(QPointF(v.x, -v.y)))
            {
                if(state)
                {
                    modelData()->vertices[i].selected = false;
                    verts.append(qMakePair(i, false));

                    any = true;
                }
            }
        }
    }

    if(elements.contains(ModelElement::Face))
    {
        for(int i = 0; i < modelData()->faces.count(); ++i)
        {
            bool state = modelData()->faces[i].selected;

            Vec3 a = transformCoord(modelData()->model()->faceVertex(i, 0).pos, transform);
            Vec3 b = transformCoord(modelData()->model()->faceVertex(i, 1).pos, transform);
            Vec3 c = transformCoord(modelData()->model()->faceVertex(i, 2).pos, transform);

            QPolygonF tri;
            tri << QPointF(a.x, -a.y) << QPointF(b.x, -b.y) << QPointF(c.x, -c.y);

            if(!tri.intersected(rect).isEmpty())
            {
                if(state)
                {
                    bool intersects = true;

                    if(frontOnly && windingOrder(a, b, c) >= 0)
                    {
                        intersects = false;
                    }

                    if(intersects)
                    {
                        modelData()->faces[i].selected = false;
                        faces.append(qMakePair(i, false));

                        any = true;
                    }
                }
            }
        }
    }

    if(elements.contains(ModelElement::Joint))
    {
        for(int i = 0; i < modelData()->joints.count(); ++i)
        {
            bool state = modelData()->joints[i].selected;

            if(jointIntersects(i, jointRadius, rect, worldMatrix, transform))
            {
                if(state)
                {
                    modelData()->joints[i].selected = false;
                    joints.append(qMakePair(i, false));

                    any = true;
                }
            }
        }
    }

    if(any)
    {
        modelData()->model()->changeSelection();
    }
}

void
SelectCommand::selectAll()
{
    bool any = false;

    for(int i = 0; i < modelData()->vertices.count(); ++i)
    {
        if(!modelData()->vertices[i].selected && !modelData()->vertices[i].hide)
        {
            modelData()->vertices[i].selected = true;
            verts.append(qMakePair(i, true));

            any = true;
        }
    }

    for(int i = 0; i < modelData()->faces.count(); ++i)
    {
        if(!modelData()->faces[i].selected && !modelData()->faces[i].hide)
        {
            modelData()->faces[i].selected = true;
            faces.append(qMakePair(i, true));

            any = true;
        }
    }

    for(int i = 0; i < modelData()->joints.count(); ++i)
    {
        if(!modelData()->joints[i].selected && !modelData()->joints[i].hide)
        {
            modelData()->joints[i].selected = true;
            joints.append(qMakePair(i, true));

            any = true;
        }
    }

    if(any)
    {
        modelData()->model()->changeSelection();
    }
}

void
SelectCommand::selectNone()
{
    bool any = false;

    for(int i = 0; i < modelData()->vertices.count(); ++i)
    {
        if(modelData()->vertices[i].selected)
        {
            modelData()->vertices[i].selected = false;
            verts.append(qMakePair(i, false));

            any = true;
        }
    }

    for(int i = 0; i < modelData()->faces.count(); ++i)
    {
        if(modelData()->faces[i].selected)
        {
            modelData()->faces[i].selected = false;
            faces.append(qMakePair(i, false));

            any = true;
        }
    }

    for(int i = 0; i < modelData()->joints.count(); ++i)
    {
        if(modelData()->joints[i].selected)
        {
            modelData()->joints[i].selected = false;
            joints.append(qMakePair(i, false));

            any = true;
        }
    }

    if(any)
    {
        modelData()->model()->changeSelection();
    }
}

void
SelectCommand::selectInvert()
{
    bool any = false;

    for(int i = 0; i < modelData()->vertices.count(); ++i)
    {
        if(!modelData()->vertices[i].hide)
        {
            bool state = !modelData()->vertices[i].selected;

            modelData()->vertices[i].selected = state;
            verts.append(qMakePair(i, state));

            any = true;
        }
    }

    for(int i = 0; i < modelData()->faces.count(); ++i)
    {
        if(!modelData()->faces[i].hide)
        {
            bool state = !modelData()->faces[i].selected;

            modelData()->faces[i].selected = state;
            faces.append(qMakePair(i, state));

            any = true;
        }
    }

    for(int i = 0; i < modelData()->joints.count(); ++i)
    {
        if(!modelData()->joints[i].hide)
        {
            bool state = !modelData()->joints[i].selected;

            modelData()->joints[i].selected = state;
            joints.append(qMakePair(i, state));

            any = true;
        }
    }

    if(any)
    {
        modelData()->model()->changeSelection();
    }
}

void
SelectCommand::selectVertices(const QVector<int> &v)
{
    foreach(int i, v)
    {
        Assert(i >= 0 && i < modelData()->vertices.count());

        if(!modelData()->vertices[i].hide)
        {
            modelData()->vertices[i].selected = true;
            verts.append(qMakePair(i, true));
        }
    }

    if(v.count())
    {
        modelData()->model()->changeSelection();
    }
}

void
SelectCommand::selectFaces(const QVector<int> &v)
{
    foreach(int i, v)
    {
        Assert(i >= 0 && i < modelData()->faces.count());

        if(!modelData()->faces[i].hide)
        {
            modelData()->faces[i].selected = true;
            faces.append(qMakePair(i, true));
        }
    }

    if(v.count())
    {
        modelData()->model()->changeSelection();
    }
}

void
SelectCommand::selectJoints(const QVector<int> &v)
{
    foreach(int i, v)
    {
        Assert(i >= 0 && i < modelData()->joints.count());

        if(!modelData()->joints[i].hide)
        {
            modelData()->joints[i].selected = true;
            joints.append(qMakePair(i, true));
        }
    }

    if(v.count())
    {
        modelData()->model()->changeSelection();
    }
}

void
SelectCommand::setJointSelection(const QVector<int> &v)
{
    bool any = false;

    for(int i = 0; i < modelData()->joints.count(); ++i)
    {
        Assert(i >= 0 && i < modelData()->joints.count());

        if(!modelData()->joints[i].hide)
        {
            if(v.contains(i))
            {
                if(!modelData()->joints[i].selected)
                {
                    modelData()->joints[i].selected = true;
                    joints.append(qMakePair(i, true));

                    any = true;
                }
            }

            if(!v.contains(i))
            {
                if(modelData()->joints[i].selected)
                {
                    modelData()->joints[i].selected = false;
                    joints.append(qMakePair(i, false));

                    any = true;
                }
            }
        }
    }

    if(any)
    {
        modelData()->model()->changeSelection();
    }
}

bool
SelectCommand::triangleIntersects(int i, const QRectF &rect, const Matrix &transform, bool frontOnly) const
{
    Vec3 a = transformCoord(modelData()->model()->faceVertex(i, 0).pos, transform);
    Vec3 b = transformCoord(modelData()->model()->faceVertex(i, 1).pos, transform);
    Vec3 c = transformCoord(modelData()->model()->faceVertex(i, 2).pos, transform);

    QPolygonF tri;
    tri << QPointF(a.x, -a.y) << QPointF(b.x, -b.y) << QPointF(c.x, -c.y);

    if(!tri.intersected(rect).isEmpty())
    {
        if(frontOnly && windingOrder(a, b, c) >= 0)
        {
            return false;
        }

        return true;
    }

    return false;
}

bool
SelectCommand::jointIntersects(int i, float jointRadius, const QRectF &rect, const Matrix &worldMatrix, const Matrix &transform) const
{
    Vec3 pos = modelData()->model()->joint(i).pos;
    if(modelData()->model()->mode() == Mode::Animate)
    {
        pos = modelData()->model()->transformedJointPosition(i);
    }

    Vec3 p = transformCoord(pos, transform);
    Vec3 n = transformNormal(Vec3(0, jointRadius, 0), inverseMatrix(worldMatrix));

    float radius = vectorLength(transformCoord(pos + n, transform) - p);

    QPolygonF circle;
    for(float a = 0; a < pi() * 2.0f; a += pi() / 8)
    {
        circle.append(QPointF(sinf(a) * radius, cosf(a) * radius) + QPointF(p.x, -p.y));
    }

    return !circle.intersected(rect).isEmpty();
}
