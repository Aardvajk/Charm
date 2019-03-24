#include "Model.h"

#include "debug/Assert.h"

#include "common/Color.h"

#include "command/Command.h"

#include "maths/Quaternion.h"

#include "undo/UndoList.h"

#include "actions/Action.h"
#include "actions/ActionList.h"

Model::Model(ActionList *actions, QObject *parent)
    : QObject(parent),
      data(this),
      faceReferenceCache(this),
      spatialCache(this),
      jointCache(this),
      selectionCache(this),
      matrixCache(this),
      groupCache(this),
      anySel(false),
      modeType(Mode::Null),
      anchorType(Anchor::Null),
      poseChangedFlag(false),
      useKey(false),
      showCursorFlag(false),
      showNormalsFlag(false),
      forceSave(false)
{
    undoList = new UndoList(this);

    connect(undoList, SIGNAL(undoStateChanged()), SLOT(undoStateChanged()));
    connect(undoList, SIGNAL(modifiedStateChanged(bool)), SIGNAL(modifiedStateChanged(bool)));
    connect(undoList, SIGNAL(undoTriggered()), SIGNAL(undoTriggered()));

    undoAction = actions->add("Model", "Undo");
    undoAction->setText("&Undo");
    undoAction->setShortcut(QKeySequence("Ctrl+Z"));

    connect(undoAction, SIGNAL(triggered()), undoList, SLOT(undo()));

    redoAction = actions->add("Model", "Redo");
    redoAction->setText("&Redo");
    redoAction->setShortcut(QKeySequence("Ctrl+Y"));

    connect(redoAction, SIGNAL(triggered()), undoList, SLOT(redo()));

    undoStateChanged();
}

bool
Model::isEmpty() const
{
    return data.isEmpty();
}

void
Model::beginCommand(Command *command)
{
    Assert(command);

    command->begin(&data);
}

void
Model::endCommand(Command *command)
{
    if(command->isValid())
    {
        undoList->addCommand(command);
    }
    else
    {
        delete command;
    }
}

Vertex
Model::vertex(int index) const
{
    Assert(index >= 0 && index < data.vertices.count());

    return data.vertices[index];
}

Vertex
Model::faceVertex(int face, int vertex) const
{
    Assert(face >= 0 && face < data.faces.count());
    Assert(vertex >= 0 && vertex < 3);

    int v = data.faces[face].indices[vertex];

    Assert(v >= 0 && v < data.vertices.count());

    return data.vertices[v];
}

Face
Model::face(int index) const
{
    Assert(index >= 0 && index < data.faces.count());

    return data.faces[index];
}

Vec3
Model::faceNormal(int face) const
{
    Assert(face >= 0 && face < data.faces.count());

    Vec3 v0 = transformedVertexPosition(data.faces[face].indices[0]);
    Vec3 v1 = transformedVertexPosition(data.faces[face].indices[1]);
    Vec3 v2 = transformedVertexPosition(data.faces[face].indices[2]);

    return normalizeVector(crossVectors(v1 - v0, v2 - v0));
}

Vec3
Model::vertexNormal(int vertex) const
{
    Assert(vertex >= 0 && vertex < data.vertices.count());

    const Vertex &v = data.vertices[vertex];
    if(v.manualNormal.valid())
    {
        return v.manualNormal.value();
    }

    const QVector<int> &faces = facesContainingVertex(vertex);
    if(faces.empty())
    {
        return Vec3(0, 0, 0);
    }

    Vec3 sharedNormal(0, 0, 0);

    foreach(int k, faces)
    {
        sharedNormal += faceNormal(k);
    }

    sharedNormal /= faces.count();

    return normalizeVector(sharedNormal);
}

bool
Model::isFaceDegenerate(int face) const
{
    Assert(face >= 0 && face < data.faces.count());

    for(int i = 0; i < 3; ++i)
    {
        int j = (i == 2 ? 0 : i + 1);

        if(vectorLength(data.vertices[data.faces[face].indices[i]].pos - data.vertices[data.faces[face].indices[j]].pos) < 0.001f)
        {
            return true;
        }
    }

    return false;
}

QVector<int>
Model::facesContainingVertex(int vertex) const
{
    return faceReferenceCache.faces(vertex);
}

Joint
Model::joint(int index) const
{
    Assert(index >= 0 && index < data.joints.count());

    return data.joints[index];
}

Animation
Model::animation(int index) const
{
    Assert(index >= 0 && index < data.animations.count());

    return data.animations[index];
}

Group
Model::group(int index) const
{
    Assert(index >= 0 && index < data.groups.count());

    return data.groups[index];
}

QColor
Model::palette(int index) const
{
    if(index < 0 || index >= data.palette.count())
    {
        return QColor(255, 255, 255);
    }

    return data.palette[index];
}

QVector<int>
Model::selectedVertices() const
{
    return selectionCache.vertices();
}

QVector<int>
Model::selectedFaces() const
{
    return selectionCache.faces();
}

QVector<int>
Model::selectedJoints() const
{
    return selectionCache.joints();
}

QVector<int>
Model::selectedFacesContainingVertex(int vertex) const
{
    QVector<int> f = faceReferenceCache.faces(vertex);
    QVector<int> s;

    for(int i = 0; i < f.count(); ++i)
    {
        Assert(f[i] >= 0 && f[i] < data.faces.count());

        if(data.faces[f[i]].selected)
        {
            s.append(f[i]);
        }
    }

    return s;
}

QVector<int>
Model::equivalentVertices(int vertex) const
{
    return spatialCache.vertices(vertex);
}

bool
Model::selectedVerticesAllEquivalent() const
{
    if(selectedVertices().count())
    {
        int index = selectedVertices().front();
        QVector<int> equivs = equivalentVertices(index);

        for(int i = 1; i < selectedVertices().count(); ++i)
        {
            if(!equivs.contains(selectedVertices()[i])) return false;
        }

        return true;
    }

    return false;
}

bool
Model::vertexGroupIncludesSelected(int vertex) const
{
    return spatialCache.selected(vertex);
}

QVector<int>
Model::verticesInSelection() const
{
    return selectionCache.verticesInSelection();
}

D3DCOLOR
Model::vertexInfluencedColor(int vertex) const
{
    D3DXCOLOR color = D3DCOLOR_XRGB(0, 0, 0);
    bool any = false;

    for(int i = 0; i < Vertex::maximumInfluences; ++i)
    {
        VertexInfluence v = data.vertices[vertex].influences[i];

        if(v.joint >= 0 && data.joints[v.joint].selected)
        {
            color += (D3DXCOLOR(D3DCOLOR_XRGB(255, 0, 0)) * v.weight) + (D3DXCOLOR(D3DCOLOR_XRGB(0, 0, 255)) * (1.0f - v.weight));
            any = true;
        }
    }

    if(!any)
    {
        return D3DCOLOR_XRGB(255, 255, 255);
    }

    return color;
}

QVector<int>
Model::jointsInSelection() const
{
    return selectionCache.jointsInSelection();
}

QVector<int>
Model::jointChildren(int index) const
{
    return jointCache.children(index);
}

bool
Model::isJointChild(int parent, int child) const
{
    foreach(int index, jointCache.children(parent))
    {
        if(index == child || isJointChild(index, child))
        {
            return true;
        }
    }

    return false;
}

Matrix
Model::jointMatrix(int index) const
{
    Assert(index >= 0 && index < data.joints.count());

    return matrixCache.matrix(index, key, useKey);
}

Vec3
Model::transformedJointPosition(int index) const
{
    Assert(index >= 0 && index < data.joints.count());
    return transformCoord(data.joints[index].pos, jointMatrix(index));
}

Vec3
Model::transformedVertexPosition(int index) const
{
    Assert(index >= 0 && index < data.vertices.count());

    if(modeType != Mode::Animate)
    {
        return data.vertices[index].pos;
    }

    Vec3 pos(0, 0, 0);
    bool any = false;

    Vertex v = data.vertices[index];

    for(int j = 0; j < Vertex::maximumInfluences; ++j)
    {
        if(v.influences[j].joint >= 0)
        {
            pos += transformCoord(v.pos, matrixCache.matrix(v.influences[j].joint, key, useKey)) * v.influences[j].weight;
            any = true;
        }
    }

    if(!any)
    {
        return v.pos;
    }

    return pos;
}

int
Model::groupIndex(int id) const
{
    Assert(groupCache.containsId(id));

    return groupCache.index(id);
}

int
Model::groupCount(int id) const
{
    Assert(groupCache.containsId(id));

    return groupCache.count(id);
}

Vec3
Model::selectionCentre() const
{
    return selectionCache.selectionCentre();
}

Vec3
Model::cursor() const
{
    return data.cursor;
}

Vec3
Model::referencePoint() const
{
    switch(anchorType)
    {
        case Anchor::Selection: return selectionCache.selectionCentre();
        case Anchor::Cursor:    return data.cursor;
        case Anchor::Origin:    return Vec3(0, 0, 0);

        default: break;
    }

    return Vec3(0, 0, 0);
}

bool
Model::modified() const
{
    return undoList->isModified() || forceSave;
}

void
Model::clear()
{
    data.clear();
    undoList->clear();

    change();
    changeAnimations();
    changeGroups();
}

bool
Model::loadFromFile(const QString &path)
{
    ModelData temp(this);

    if(!temp.loadFromFile(path))
    {
        return false;
    }

    data = temp;
    undoList->clear();

    change();
    changeAnimations();
    changeGroups();
    changePalette();

    return true;
}

bool
Model::saveToFile(const QString &path) const
{
    return data.saveToFile(path);
}

void
Model::setSavePoint()
{
    undoList->setSavePoint();

    if(forceSave)
    {
        forceSave = false;
        emit modifiedStateChanged(false);
    }
}

void
Model::setPath(const QString &path)
{
    if(filePath != path)
    {
        filePath = path;
        emit pathChanged(filePath);
    }
}

void
Model::setLastExportDetails(const ExportDetails &details)
{
    if(data.lastExportDetails != details)
    {
        data.lastExportDetails = details;
        forceSave = true;

        emit modifiedStateChanged(true);
    }
}

void
Model::change()
{
    updateCaches();
    emit changed();
}

void
Model::changeSelection()
{
    updateCaches();
    emit selectionChanged();
}

void
Model::changeCursor()
{
    updateCaches();
    emit cursorChanged();
}

void
Model::changePose()
{
    updateCaches();
    emit poseChanged();
}

void
Model::changeAnimations()
{
    emit animationsChanged();
}

void
Model::changeAnimation(int index)
{
    emit animationChanged(index);
}

void
Model::changeGroups()
{
    groupCache.invalidate();
    emit groupsChanged();
}

void
Model::changePalette()
{
    emit paletteChanged();
}

void
Model::remapJoints(const QMap<int, int> &indices)
{
    emit jointsRemapped(indices);
}

void
Model::setKeyFrame(const KeyFrame &keyFrame)
{
    key = keyFrame;

    for(int i = 0; i < keyFrame.count(); ++i)
    {
        data.joints[i].transform = keyFrame.transform(i);
    }

    matrixCache.invalidate();
}

void
Model::undoStateChanged()
{
    if(undoList->canUndo())
    {
        undoAction->setText("Undo " + undoList->lastCommandName());
        undoAction->setEnabled(true);
    }
    else
    {
        undoAction->setText("Undo");
        undoAction->setEnabled(false);
    }

    if(undoList->canRedo())
    {
        redoAction->setText("Redo " + undoList->nextCommandName());
        redoAction->setEnabled(true);
    }
    else
    {
        redoAction->setText("Redo");
        redoAction->setEnabled(false);
    }
}

void
Model::lockAxis(int axis, bool state)
{
    emit axisLocked(axis, state);
}

void
Model::setMode(Mode::Type type)
{
    if(type != modeType)
    {
        modeType = type;

        updateCaches();

        emit modeChanged();
    }
}

void
Model::setAnchor(Anchor::Type type)
{
    if(type != anchorType)
    {
        anchorType = type;

        updateCaches();

        emit anchorChanged();
    }
}

void
Model::setPoseChangedFromKeyFrame(bool state)
{
    poseChangedFlag = state;
}

void
Model::setUseKeyFrame(bool state)
{
    useKey = state;
    matrixCache.invalidate();
}

void
Model::setAlwaysShowCursor(bool state)
{
    showCursorFlag = state;
}

void
Model::setShowNormals(bool state)
{
    if(state != showNormalsFlag)
    {
        showNormalsFlag = state;
        emit changed();
    }
}

void
Model::updateCaches()
{
    faceReferenceCache.invalidate();
    spatialCache.invalidate();
    jointCache.invalidate();
    selectionCache.invalidate();
    matrixCache.invalidate();
    groupCache.invalidate();

    bool any = selectionCache.anySelection();
    if(any != anySel)
    {
        anySel = any;
    }
}
