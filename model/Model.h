#ifndef MODEL_H
#define MODEL_H

#include <QtCore/QObject>
#include <QtCore/QMap>

#include "common/Mode.h"
#include "common/Anchor.h"
#include "common/IdIssuer.h"

#include "maths/Vec3.h"
#include "maths/Matrix.h"

#include "model/ModelData.h"

#include "model/caches/FaceReferenceCache.h"
#include "model/caches/SpatialCache.h"
#include "model/caches/JointRelationCache.h"
#include "model/caches/SelectionCache.h"
#include "model/caches/JointMatrixCache.h"
#include "model/caches/GroupCache.h"

class Command;

class Action;
class ActionList;

class UndoList;

class Model : public QObject
{
    Q_OBJECT

public:
    Model(ActionList *actions, QObject *parent = 0);

    bool
    isEmpty() const;

    void
    beginCommand(Command *command);

    void
    endCommand(Command *command);

    int
    vertexCount() const { return data.vertices.count(); }

    Vertex
    vertex(int index) const;

    Vertex
    faceVertex(int face, int vertex) const;

    int
    faceCount() const { return data.faces.count(); }

    Face
    face(int index) const;

    Vec3
    faceNormal(int face) const;

    Vec3
    vertexNormal(int vertex) const;

    bool
    isFaceDegenerate(int face) const;

    QVector<int>
    facesContainingVertex(int vertex) const;

    int
    jointCount() const { return data.joints.count(); }

    Joint
    joint(int index) const;

    int
    animationCount() const { return data.animations.count(); }

    Animation
    animation(int index) const;

    int
    groupCount() const { return data.groups.count(); }

    Group
    group(int index) const;

    int
    paletteCount() const { return data.palette.count(); }

    QColor
    palette(int index) const;

    bool
    anySelection() const { return anySel; }

    QVector<int>
    selectedVertices() const;

    QVector<int>
    selectedFaces() const;

    QVector<int>
    selectedJoints() const;

    QVector<int>
    selectedFacesContainingVertex(int vertex) const;

    QVector<int>
    equivalentVertices(int vertex) const;

    bool
    selectedVerticesAllEquivalent() const;

    bool
    vertexGroupIncludesSelected(int vertex) const;

    QVector<int>
    verticesInSelection() const;

    D3DCOLOR
    vertexInfluencedColor(int vertex) const;

    QVector<int>
    jointsInSelection() const;

    QVector<int>
    jointChildren(int index) const;

    bool
    isJointChild(int parent, int child) const;

    Matrix
    jointMatrix(int index) const;

    Vec3
    transformedJointPosition(int index) const;

    Vec3
    transformedVertexPosition(int index) const;

    int
    groupIndex(int id) const;

    int
    groupCount(int id) const;

    Vec3
    selectionCentre() const;

    Vec3
    cursor() const;

    Vec3
    referencePoint() const;

    bool
    modified() const;

    void
    clear();

    bool
    loadFromFile(const QString &path);

    bool
    saveToFile(const QString &path) const;

    void
    setSavePoint();

    QString
    path() const { return filePath; }

    void
    setPath(const QString &path);

    ExportDetails
    lastExportDetails() const { return data.lastExportDetails; }

    void
    setLastExportDetails(const ExportDetails &details);

    int
    hide() const { return data.hide; }

    Mode::Type
    mode() const { return modeType; }

    Anchor::Type
    anchor() const { return anchorType; }

    void
    change();

    void
    changeSelection();

    void
    changeCursor();

    void
    changePose();

    void
    changeAnimations();

    void
    changeAnimation(int index);

    void
    changeGroups();

    void
    changePalette();

    void
    remapJoints(const QMap<int, int> &indices);

    bool
    poseChangedFromKeyFrame() const { return poseChangedFlag; }

    void
    setKeyFrame(const KeyFrame &keyFrame);

    bool
    useKeyFrame() const { return useKey; }

    int
    nextGroupId(){ return groupIds.id(); }

    void
    removeGroupId(int id){ groupIds.remove(id); }

    bool
    alwaysShowCursor() const { return showCursorFlag; }

    bool
    showNormals() const { return showNormalsFlag; }

signals:
    void
    changed();

    void
    selectionChanged();

    void
    cursorChanged();

    void
    poseChanged();

    void
    modifiedStateChanged(bool state);

    void
    pathChanged(const QString &path);

    void
    undoTriggered();

    void
    animationsChanged();

    void
    animationChanged(int index);

    void
    groupsChanged();

    void
    paletteChanged();

    void
    axisLocked(int axis, bool state);

    void
    modeChanged();

    void
    anchorChanged();

    void
    jointsRemapped(const QMap<int, int> &indices);

public slots:
    void
    lockAxis(int axis, bool state);

    void
    setMode(Mode::Type type);

    void
    setAnchor(Anchor::Type type);

    void
    setPoseChangedFromKeyFrame(bool state);

    void
    setUseKeyFrame(bool state);

    void
    setAlwaysShowCursor(bool state);

    void
    setShowNormals(bool state);

private slots:
    void
    undoStateChanged();

private:
    void
    updateCaches();

    ModelData data;

    FaceReferenceCache faceReferenceCache;
    SpatialCache spatialCache;
    JointRelationCache jointCache;
    SelectionCache selectionCache;
    JointMatrixCache matrixCache;
    GroupCache groupCache;

    UndoList *undoList;

    Action *undoAction;
    Action *redoAction;

    QString filePath;

    bool anySel;
    bool cursorVisible;

    Mode::Type modeType;
    Anchor::Type anchorType;

    bool poseChangedFlag;
    bool useKey;

    KeyFrame key;

    IdIssuer groupIds;

    bool showCursorFlag;
    bool showNormalsFlag;
};

#endif // MODEL_H
