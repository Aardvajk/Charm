#include "DeleteCommand.h"

#include <QtCore/QSet>
#include <QtCore/QMap>

#include "debug/Assert.h"
#include "debug/FunctionTimer.h"

#include "model/Model.h"
#include "model/ModelData.h"

DeleteCommand::DeleteCommand(Model *model, QObject *parent)
    : Command(model, parent)
{
}

QString
DeleteCommand::name() const
{
    return "Delete";
}

bool
DeleteCommand::isValid() const
{
    return verts.count() || faces.count() || joints.count();
}

bool
DeleteCommand::doesModifyPersistentState() const
{
    return true;
}

void
DeleteCommand::undo()
{
    TimeFunction;

    QVector<int> mapping;
    for(int i = 0; i < modelData()->vertices.count(); ++i)
    {
        mapping.append(i);
    }

    QMap<int, Vertex> vertMap;
    for(int i = 0; i < verts.count(); ++i)
    {
        vertMap[verts[i].first] = verts[i].second;
    }

    for(QMap<int, Vertex>::const_iterator m = vertMap.begin(); m != vertMap.end(); ++m)
    {
        for(int i = 0; i < mapping.count(); ++i)
        {
            if(mapping[i] >= m.key()) ++mapping[i];
        }

        modelData()->vertices.insert(m.key(), m.value());
    }

    for(int i = 0; i < modelData()->faces.count(); ++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            modelData()->faces[i].indices[j] = mapping[modelData()->faces[i].indices[j]];
        }
    }

    QMap<int, Face> faceMap;
    for(int i = 0; i < faces.count(); ++i)
    {
        faceMap[faces[i].first] = faces[i].second;
    }

    for(QMap<int, Face>::const_iterator m = faceMap.begin(); m != faceMap.end(); ++m)
    {
        modelData()->faces.insert(m.key(), m.value());
    }

    for(int i = 0; i < jointParents.count(); ++i)
    {
        modelData()->joints[jointParents[i].first].details.parent = jointParents[i].second;
    }

    for(int i = 0; i < jointPairs.count(); ++i)
    {
        modelData()->joints[jointPairs[i].first].details.paired = jointPairs[i].second;
    }

    QMap<int, int> jointMapping;
    for(int i = 0; i < modelData()->joints.count(); ++i)
    {
        jointMapping[i] = i;
    }

    for(int i = 0; i < joints.count(); ++i)
    {
        foreach(int j, jointMapping.keys())
        {
            if(jointMapping[j] >= joints[i].first)
            {
                ++jointMapping[j];
            }
        }

        modelData()->joints.insert(joints[i].first, joints[i].second);
    }

    for(int i = 0; i < animations.count(); ++i)
    {
        modelData()->animations[animations[i].index] = animations[i].old;
    }

    for(int i = 0; i < influences.count(); ++i)
    {
        modelData()->vertices[influences[i].first].influences[influences[i].second.first] = influences[i].second.second;
    }

    modelData()->model()->remapJoints(jointMapping);
    modelData()->model()->changeSelection();
    modelData()->model()->change();
    modelData()->model()->changeGroups();
}

void
DeleteCommand::redo()
{
    activate();
}

void
DeleteCommand::setVertices(const QVector<int> &v)
{
    foreach(int i, v)
    {
        verts.append(qMakePair(i, modelData()->vertices[i]));
    }
}

void
DeleteCommand::setFaces(const QVector<int> &v)
{
    foreach(int i, v)
    {
        faces.append(qMakePair(i, modelData()->faces[i]));
    }
}

void
DeleteCommand::setJoints(const QVector<int> &v)
{
    foreach(int i, v)
    {
        jointIndices.append(i);
        joints.append(qMakePair(i, modelData()->joints[i]));
    }
}

void
DeleteCommand::activate()
{
    TimeFunction;

    QSet<int> jointMap;
    for(int i = 0; i < joints.count(); ++i)
    {
        jointMap << joints[i].first;
    }

    influences.clear();

    for(int i = 0; i < modelData()->vertices.count(); ++i)
    {
        for(int j = 0; j < Vertex::maximumInfluences; ++j)
        {
            if(jointMap.contains(modelData()->vertices[i].influences[j].joint))
            {
                influences.append(qMakePair(i, qMakePair(j, modelData()->vertices[i].influences[j])));

                modelData()->vertices[i].influences[j].joint = -1;
                modelData()->vertices[i].influences[j].weight = 0;
            }
        }
    }

    QSet<int> vertMap;
    QSet<int> faceMap;

    for(int i = 0; i < verts.count(); ++i)
    {
        vertMap << verts[i].first;

        foreach(int i, modelData()->model()->facesContainingVertex(verts[i].first))
        {
            faceMap << i;
            faces.append(qMakePair(i, modelData()->faces[i]));
        }
    }

    for(int i = 0; i < faces.count(); ++i)
    {
        faceMap << faces[i].first;
    }

    QVector<Face> faceBuffer;
    for(int i = 0; i < modelData()->faces.count(); ++i)
    {
        if(!faceMap.contains(i))
        {
            faceBuffer.append(modelData()->faces[i]);
        }
    }

    modelData()->faces.swap(faceBuffer);

    QVector<Vertex> vertBuffer;
    QMap<int, int> mapping;
    for(int i = 0; i < modelData()->vertices.count(); ++i)
    {
        if(!vertMap.contains(i))
        {
            vertBuffer.append(modelData()->vertices[i]);
            mapping[i] = vertBuffer.count() - 1;
        }
    }

    modelData()->vertices.swap(vertBuffer);

    for(int i = 0; i < modelData()->faces.count(); ++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            QMap<int, int>::iterator m = mapping.find(modelData()->faces[i].indices[j]);
            if(m != mapping.end())
            {
                modelData()->faces[i].indices[j] = m.value();
            }
        }
    }

    mapping.clear();

    QVector<Joint> jointBuffer;
    for(int i = 0; i < modelData()->joints.count(); ++i)
    {
        if(!jointMap.contains(i))
        {
            jointBuffer.append(modelData()->joints[i]);
            mapping[i] = jointBuffer.count() - 1;
        }
    }

    modelData()->joints.swap(jointBuffer);

    jointParents.clear();
    jointPairs.clear();

    for(int i = 0; i < modelData()->joints.count(); ++i)
    {
        int p = modelData()->joints[i].details.parent;

        if(p >= 0)
        {
            if(jointMap.contains(p))
            {
                jointParents.append(qMakePair(i, modelData()->joints[i].details.parent));
                modelData()->joints[i].details.parent = -1;
            }
            else
            {
                QMap<int, int>::iterator m = mapping.find(p);
                if(m != mapping.end())
                {
                    jointParents.append(qMakePair(i, modelData()->joints[i].details.parent));
                    modelData()->joints[i].details.parent = m.value();
                }
            }
        }

        p = modelData()->joints[i].details.paired;

        if(p >= 0)
        {
            if(jointMap.contains(p))
            {
                jointPairs.append(qMakePair(i, modelData()->joints[i].details.paired));
                modelData()->joints[i].details.paired = -1;
            }
            else
            {
                QMap<int, int>::iterator m = mapping.find(p);
                if(m != mapping.end())
                {
                    jointPairs.append(qMakePair(i, modelData()->joints[i].details.paired));
                    modelData()->joints[i].details.paired = m.value();
                }
            }
        }
    }

    animations.clear();
    if(jointIndices.count())
    {
        for(int i = 0; i < modelData()->animations.count(); ++i)
        {
            Animation animation = modelData()->animations[i];
            animation.removeJoints(jointIndices);

            animations.append(StoredAnimation(i, modelData()->animations[i], animation));
        }
    }

    modelData()->model()->remapJoints(mapping);
    modelData()->model()->changeSelection();
    modelData()->model()->change();
    modelData()->model()->changeGroups();
}
