#include "SelectionCache.h"

#include "model/Model.h"

SelectionCache::SelectionCache(const Model *model)
    : model(model),
      valid(false),
      centre(0, 0, 0)
{
}

void
SelectionCache::invalidate()
{
    valid = false;
}

const QVector<int>&
SelectionCache::vertices() const
{
    if(!valid)
    {
        refresh();
        valid = true;
    }

    return selectedVerts;
}

const QVector<int>&
SelectionCache::faces() const
{
    if(!valid)
    {
        refresh();
        valid = true;
    }

    return selectedFaces;
}

const QVector<int>&
SelectionCache::joints() const
{
    if(!valid)
    {
        refresh();
        valid = true;
    }

    return selectedJoints;
}

const QVector<int>&
SelectionCache::verticesInSelection() const
{
    if(!valid)
    {
        refresh();
        valid = true;
    }

    return selectedAllVerts;
}

const QVector<int>&
SelectionCache::jointsInSelection() const
{
    if(!valid)
    {
        refresh();
        valid = true;
    }

    return selectedAllJoints;
}

Vec3
SelectionCache::selectionCentre() const
{
    if(!valid)
    {
        refresh();
        valid = true;
    }

    return centre;
}

bool
SelectionCache::anySelection() const
{
    if(!valid)
    {
        refresh();
        valid = true;
    }

    return selectedVerts.count() || selectedFaces.count() || selectedJoints.count();
}

void
SelectionCache::refresh() const
{
    selectedVerts.clear();
    selectedAllVerts.clear();

    QSet<int> allVerts;

    for(int i = 0; i < model->vertexCount(); ++i)
    {
        if(model->vertex(i).selected)
        {
            selectedVerts.append(i);
            allVerts.insert(i);
        }
    }

    selectedFaces.clear();

    for(int i = 0; i < model->faceCount(); ++i)
    {
        if(model->face(i).selected)
        {
            selectedFaces.append(i);
            allVerts.insert(model->face(i).indices[0]);
            allVerts.insert(model->face(i).indices[1]);
            allVerts.insert(model->face(i).indices[2]);
        }
    }

    QSet<int> allJoints;

    selectedJoints.clear();

    for(int i = 0; i < model->jointCount(); ++i)
    {
        if(model->joint(i).selected)
        {
            selectedJoints.append(i);
            allJoints.insert(i);

            addJointChildren(i, allJoints);
        }
    }

    selectedAllJoints.clear();

    foreach(int i, allJoints)
    {
        selectedAllJoints.append(i);
    }

    foreach(int i, allVerts)
    {
        selectedAllVerts.append(i);
    }

    calculateAverageCentre(allVerts);
}

void
SelectionCache::calculateAverageCentre(const QSet<int> &allVerts) const
{
    centre = Vec3(0, 0, 0);

    foreach(int i, allVerts)
    {
        centre += model->vertex(i).pos;
    }

    if(model->mode() == Mode::Model)
    {
        foreach(int i, selectedJoints)
        {
//            if(model->joint(i).details.parent >= 0)
//            {
//                centre += model->joint(model->joint(i).details.parent).pos;
//            }
//            else
            {
                centre += model->joint(i).pos;
            }
        }
    }
    else
    {
        foreach(int i, selectedJoints)
        {
//            if(model->joint(i).details.parent >= 0)
//            {
//                centre += model->transformedJointPosition(model->joint(i).details.parent);
//            }
//            else
            {
                centre += model->transformedJointPosition(i);
            }
        }
    }

    int total = allVerts.count() + selectedJoints.count();

    if(total)
    {
        centre /= static_cast<float>(total);
    }
}

void
SelectionCache::addJointChildren(int index, QSet<int> &joints) const
{
    foreach(int i, model->jointChildren(index))
    {
        joints.insert(i);
        addJointChildren(i, joints);
    }
}
