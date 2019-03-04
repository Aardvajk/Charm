#ifndef MODELVIEW_H
#define MODELVIEW_H

#include "maths/Vec3.h"
#include "maths/Matrix.h"

#include "views/ModelViewSettings.h"

#include "graphics/GraphicsWidget.h"

#include "common/Projection.h"
#include "common/Rotation.h"
#include "common/Shade.h"
#include "common/Axis.h"

#include "model/Joint.h"

#include <QtCore/QPointF>

class Graphics;
class Model;
class ModelBuffers;
class VertexBuffer;
class VertexShader;
class VectorAnimation;
class Settings;

class ModelView : public GraphicsWidget
{
    Q_OBJECT

public:
    ModelView(Graphics *graphics, Model *model, ModelBuffers *buffers, QWidget *parent = 0);

    void
    reset();

    Vec3
    position() const { return settings.pos; }

    Rotation::Type
    rotation() const { return settings.rot; }

    Vec3
    rotationVector() const { return settings.rotxy; }

    Projection::Type
    projection() const { return settings.proj; }

    bool
    wireframeRender() const { return settings.wireframe; }

    Shade::Type
    shading() const { return settings.shade; }

    Paint::Type
    painting() const { return settings.paint; }

    Axis::Type
    reflection() const { return settings.ref; }

    bool
    smoothReflectionEdges() const { return settings.smooth; }

    bool
    renderSkeleton() const { return settings.skeleton; }

    ModelBuffers*
    modelBuffers() const { return buffers; }

    QPointF
    screenToZPlane(const QPointF &p);

    Vec3
    getWorldPosition(const QPoint &point, const Vec3 &plane);

    Vec3
    getViewPoint(const QPoint &point);

    Matrix
    worldTransform() const;

    Matrix
    viewTransform() const;

    Matrix
    projectionTransform() const;

    Matrix
    transform() const;

    void
    loadSettings(Settings *settings);

    void
    saveSettings(Settings *settings) const;

signals:
    void
    mousePressed(ModelView *view, QMouseEvent *event);

    void
    mouseMoved(ModelView *view, QMouseEvent *event);

    void
    mouseReleased(ModelView *view, QMouseEvent *event);

    void
    mouseWheelEvent(ModelView *view, QWheelEvent *event);

    void
    settingsChanged();

    void
    render(ModelView *view, Graphics *graphics);

public slots:
    void
    setPosition(const Vec3 &value);

    void
    setPositionImmediate(const Vec3 &value);

    void
    setRotation(Rotation::Type value);

    void
    setRotationImmediate(Rotation::Type value);

    void
    setRotationVector(const Vec3 &value);

    void
    setProjection(Projection::Type value);

    void
    setWireframeRender(bool value);

    void
    setShading(Shade::Type value);

    void
    setPainting(Paint::Type value);

    void
    setReflection(Axis::Type axis);

    void
    setSmoothReflectionEdges(bool state);

    void
    setRenderSkeleton(bool state);

    void
    setCurrentSettingsAsDefault();

protected:
    virtual
    void
    paintEvent(QPaintEvent *event);

    virtual
    void
    mousePressEvent(QMouseEvent *event);

    virtual
    void
    mouseMoveEvent(QMouseEvent *event);

    virtual
    void
    mouseReleaseEvent(QMouseEvent *event);

    virtual
    void
    wheelEvent(QWheelEvent *event);

private slots:
    void
    rotationAnimationChanged(const Vec3 &value);

    void
    positionAnimationChanged(const Vec3 &value);

private:
    void
    renderWholeModel(const Matrix &worldMatrix, const Matrix &viewMatrix, const Matrix &projMatrix);

    void
    renderModel(const Vec3 &reflect, const Vec3 &merge, const Matrix &worldMatrix, const Matrix &viewMatrix, const Matrix &projMatrix);

    void
    renderOrthographicGrid(VertexBuffer *buffer, const Matrix &viewTransform, const Matrix &projMatrix);

    void
    renderJoint(VertexShader *shader, const Vec3 &pos, bool selected, const Matrix &worldMatrix, const Matrix &viewMatrix, const Matrix &projMatrix);

    void
    renderBone(VertexShader *shader, const Vec3 &a, const Vec3 &b, const QColor &color, const Matrix &worldMatrix, const Matrix &viewMatrix, const Matrix &projMatrix);

    void
    renderSkeleton(const Matrix &worldMatrix, const Matrix &viewMatrix, const Matrix &projMatrix);

    void
    renderCursor(VertexBuffer *buffer, const Matrix &worldMatrix, const Matrix &viewMatrix, const Matrix &projMatrix);

    Graphics *graphics;

    Model *model;
    ModelBuffers *buffers;

    ModelViewSettings settings;
    ModelViewSettings defSettings;

    float fov;

    VectorAnimation *rotationAnimation;
    VectorAnimation *positionAnimation;
};

#endif // MODELVIEW_H
