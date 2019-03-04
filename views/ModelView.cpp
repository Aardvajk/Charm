#include "ModelView.h"

#include "debug/FunctionTimer.h"

#include "settings/Settings.h"

#include "maths/Maths.h"

#include "common/VectorAnimation.h"
#include "common/Color.h"

#include "model/Model.h"
#include "model/ModelBuffers.h"

#include "buffers/Buffer.h"
#include "buffers/PreviewBuffer.h"

#include "graphics/Graphics.h"
#include "graphics/GraphicsDevice.h"
#include "graphics/VertexShader.h"
#include "graphics/BasicVertex.h"
#include "graphics/PointVertex.h"
#include "graphics/VertexBuffer.h"

namespace
{

Vec3
reflectVector(Axis::Type type)
{
    switch(type)
    {
        case Axis::X: return Vec3(-1, 1, 1);
        case Axis::Y: return Vec3(1, -1, 1);
        case Axis::Z: return Vec3(1, 1, -1);

        default: break;
    }

    return Vec3();
}

Vec3
smoothVector(Axis::Type type)
{
    switch(type)
    {
        case Axis::X: return Vec3(1, 0, 0);
        case Axis::Y: return Vec3(0, 1, 0);
        case Axis::Z: return Vec3(0, 0, 1);

        default: break;
    }

    return Vec3();
}

}

ModelView::ModelView(Graphics *graphics, Model *model, ModelBuffers *buffers, QWidget *parent)
    : GraphicsWidget(graphics->device(), parent),
      graphics(graphics),
      model(model),
      buffers(buffers),
      fov(pi() / 4)
{
    setAttribute(Qt::WA_Hover);
    setAttribute(Qt::WA_MouseTracking);

    rotationAnimation = new VectorAnimation(this);
    rotationAnimation->setDuration(100);

    connect(rotationAnimation, SIGNAL(vectorChanged(Vec3)), SLOT(rotationAnimationChanged(Vec3)));

    positionAnimation = new VectorAnimation(this);
    positionAnimation->setDuration(100);

    connect(positionAnimation, SIGNAL(vectorChanged(Vec3)), SLOT(positionAnimationChanged(Vec3)));

    defSettings = settings;
}

void
ModelView::reset()
{
    setPosition(defSettings.pos);

    if(defSettings.rot == Rotation::User)
    {
        setRotationVector(defSettings.rotxy);
    }
    else
    {
        setRotation(defSettings.rot);
    }

    settings.proj = defSettings.proj;
    settings.wireframe = defSettings.wireframe;
    settings.shade = defSettings.shade;
    settings.ref = defSettings.ref;
    settings.smooth = defSettings.smooth;
    settings.skeleton = defSettings.skeleton;
    settings.paint = defSettings.paint;
}

void
ModelView::setPosition(const Vec3 &value)
{
    if(settings.pos != value)
    {
        positionAnimation->stop();

        positionAnimation->setStartValue(settings.pos);
        positionAnimation->setEndValue(value);

        positionAnimation->start();

        emit settingsChanged();
    }
}

void
ModelView::setPositionImmediate(const Vec3 &value)
{
    if(settings.pos != value)
    {
        positionAnimation->stop();

        settings.pos = value;
        emit settingsChanged();
    }
}

void
ModelView::setRotation(Rotation::Type value)
{
    if(settings.rot != value)
    {
        settings.rot = value;

        rotationAnimation->stop();

        rotationAnimation->setStartValue(settings.rotxy);
        rotationAnimation->setEndValue(Rotation::toVector(value));

        rotationAnimation->start();

        emit settingsChanged();
    }
}

void
ModelView::setRotationImmediate(Rotation::Type value)
{
    if(settings.rot != value)
    {
        rotationAnimation->stop();

        settings.rot = value;
        settings.rotxy = Rotation::toVector(value);

        emit settingsChanged();
    }
}

void
ModelView::setRotationVector(const Vec3 &value)
{
    if(settings.rotxy != value)
    {
        rotationAnimation->stop();

        settings.rotxy = value;
        settings.rot = Rotation::User;

        emit settingsChanged();
    }
}

void
ModelView::setProjection(Projection::Type value)
{
    if(settings.proj != value)
    {
        settings.proj = value;
        emit settingsChanged();
    }
}

void
ModelView::setWireframeRender(bool value)
{
    if(settings.wireframe != value)
    {
        settings.wireframe = value;
        emit settingsChanged();
    }
}

void
ModelView::setShading(Shade::Type value)
{
    if(settings.shade != value)
    {
        settings.shade = value;
        emit settingsChanged();
    }
}

void
ModelView::setPainting(Paint::Type value)
{
    if(settings.paint != value)
    {
        settings.paint = value;
        emit settingsChanged();
    }
}

void
ModelView::setReflection(Axis::Type axis)
{
    if(settings.ref != axis)
    {
        settings.ref = axis;
        emit settingsChanged();
    }
}

void
ModelView::setSmoothReflectionEdges(bool state)
{
    if(settings.smooth != state)
    {
        settings.smooth = state;
        emit settingsChanged();
    }
}

void
ModelView::setRenderSkeleton(bool state)
{
    if(settings.skeleton != state)
    {
        settings.skeleton = state;
        emit settingsChanged();
    }
}

void
ModelView::setCurrentSettingsAsDefault()
{
    defSettings = settings;
}

QPointF
ModelView::screenToZPlane(const QPointF &p)
{
    float vx = float(size().width()) / 2;
    float vy = float(size().height()) / 2;

    float fx = (-settings.pos.z) * tan(fov / 2) * (vx / vy);
    float fy = (-settings.pos.z) * tan(fov / 2);

    float nx = p.x() - vx;
    float ny = p.y() - vy;

    nx = nx / vx;
    ny = -ny / vy;

    QPointF s(nx * fx, ny * fy);

    s.setX(s.x() + settings.pos.x);
    s.setY(s.y() + settings.pos.y);

    return s;
}

Vec3
ModelView::getWorldPosition(const QPoint &point, const Vec3 &plane)
{
    float hw = size().width() / 2.0f;
    float hh = size().height() / 2.0f;

    if(settings.proj == Projection::Orthographic)
    {
        float u = orthoUnit(settings.pos, size(), fov);

        float x = (point.x() - hw) / u;
        float y = -(point.y() - hh) / u;

        Matrix m = inverseMatrix(worldTransform() * viewTransform());

        return projectPointToPlane(transformCoord(Vec3(x, y, 0), m), plane, transformNormal(Vec3(0, 0, 1), m));
    }
    else
    {
        float f = fov / 2.0f;

        float vx = tan(f) * (point.x() - hw) / hh;
        float vy = tan(f) * -(point.y() - hh) / hh;

        float dist = 58;

        return transformCoord(Vec3(vx * dist, vy * dist, dist), inverseMatrix(worldTransform() * viewTransform()));
    }
}

Vec3
ModelView::getViewPoint(const QPoint &point)
{
    float hw = size().width() / 2.0f;
    float hh = size().height() / 2.0f;

    float x;
    float y;

    if(settings.proj == Projection::Orthographic)
    {
        float u = orthoUnit(settings.pos, size(), fov);

        x = (point.x() - hw) / u;
        y = -(point.y() - hh) / u;
    }
    else
    {
        float halfFov = fov / 2.0f;

        x = tan(halfFov) * (point.x() - hw) / hh;
        y = tan(halfFov) * -(point.y() - hh) / hh;

        x *= -settings.pos.z;
        y *= -settings.pos.z;
    }

    Matrix m = inverseMatrix(worldTransform() * viewTransform());

    return projectPointToPlane(transformCoord(Vec3(x, y, 0), m), Vec3(0, 0, 0), transformNormal(Vec3(0, 0, 1), m));
}

Matrix
ModelView::worldTransform() const
{
    Matrix rotX = rotationXMatrix(settings.rotxy.x);
    Matrix rotY = rotationYMatrix(settings.rotxy.y);

    return rotY * rotX;
}

Matrix
ModelView::viewTransform() const
{
    return lookAtMatrix(settings.pos, settings.pos + Vec3(0, 0, 1), Vec3(0, 1, 0));
}

Matrix
ModelView::projectionTransform() const
{
    if(settings.proj == Projection::Perspective)
    {
        return perspectiveMatrix(size(), fov);
    }
    else
    {
        return orthoMatrix(settings.pos, size(), fov);
    }
}

Matrix
ModelView::transform() const
{
    return worldTransform() * viewTransform() * projectionTransform();
}

void
ModelView::loadSettings(Settings *settings)
{
    this->settings.loadSettings(settings);
    emit settingsChanged();
}

void
ModelView::saveSettings(Settings *settings) const
{
    this->settings.saveSettings(settings);
}

void
ModelView::paintEvent(QPaintEvent *event)
{
    if(!isVisible() || graphics->isLost())
    {
        return;
    }

    graphics->device()->begin(this, D3DCOLOR_XRGB(120, 120, 120));

    Matrix worldMatrix = worldTransform();
    Matrix viewMatrix = viewTransform();

    Matrix projMatrix = projectionTransform();

    if(settings.proj == Projection::Orthographic && settings.rot != Rotation::User)
    {
        renderOrthographicGrid(graphics->temporaryVertexBuffer(), viewMatrix, projMatrix);
    }

    if(settings.shade != Shade::None)
    {
        renderWholeModel(worldMatrix, viewMatrix, projMatrix);
    }

    if(settings.wireframe)
    {
        graphics->setPointVertexDeclaration();

        VertexShader *shader = graphics->setWireFrameVertexShader();
        shader->setMatrix("worldviewproj", worldMatrix * viewMatrix * projMatrix);

        graphics->device()->renderLineList(buffers->wireFrameBuffer()->buffer(), sizeof(PointVertex));

        graphics->device()->getDevice()->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1, 0);

        if(settings.shade != Shade::None)
        {
            graphics->device()->enableBufferWrite(false);

            renderWholeModel(worldMatrix, viewMatrix, projMatrix);

            graphics->device()->enableBufferWrite(true);

            graphics->setPointVertexDeclaration();

            shader = graphics->setWireFrameVertexShader();
            shader->setMatrix("worldviewproj", worldMatrix * viewMatrix * projMatrix);
        }

        graphics->device()->setPointSize(3.0f);

        graphics->device()->renderPointList(buffers->pointBuffer()->buffer(), sizeof(PointVertex));
    }

    if(settings.skeleton)
    {
        renderSkeleton(worldMatrix, viewMatrix, projMatrix);
    }

    if(model->anchor() == Anchor::Cursor || model->alwaysShowCursor())
    {
        renderCursor(graphics->temporaryVertexBuffer(), worldMatrix, viewMatrix, projMatrix);
    }

    emit render(this, graphics);

    graphics->device()->end(this);
}

void
ModelView::mousePressEvent(QMouseEvent *event)
{
    emit mousePressed(this, event);
}

void
ModelView::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseMoved(this, event);
}

void
ModelView::mouseReleaseEvent(QMouseEvent *event)
{
    emit mouseReleased(this, event);
}

void
ModelView::wheelEvent(QWheelEvent *event)
{
    emit mouseWheelEvent(this, event);
}

void
ModelView::rotationAnimationChanged(const Vec3 &value)
{
    settings.rotxy = value;
}

void
ModelView::positionAnimationChanged(const Vec3 &value)
{
    settings.pos = value;
}

void
ModelView::renderWholeModel(const Matrix &worldMatrix, const Matrix &viewMatrix, const Matrix &projMatrix)
{
    Vec3 smoothAxis(0, 0, 0);
    if(settings.smooth && settings.ref != Axis::None)
    {
        smoothAxis = smoothVector(settings.ref);
    }

    renderModel(Vec3(1, 1, 1), smoothAxis, worldMatrix, viewMatrix, projMatrix);

    if(settings.ref != Axis::None)
    {
        graphics->device()->setCullMode(D3DCULL_CW);
        renderModel(reflectVector(settings.ref), smoothAxis, worldMatrix, viewMatrix, projMatrix);
        graphics->device()->setCullMode(D3DCULL_CCW);
    }
}

void
ModelView::renderModel(const Vec3 &reflect, const Vec3 &merge, const Matrix &worldMatrix, const Matrix &viewMatrix, const Matrix &projMatrix)
{
    graphics->setBasicVertexDeclaration();

    VertexShader *shader = graphics->setBasicVertexShader();
    shader->setMatrix("worldview", worldMatrix * viewMatrix);
    shader->setMatrix("worldviewproj", worldMatrix * viewMatrix * projMatrix);
    shader->setVector("reflect", reflect);
    shader->setVector("merge", merge);
    shader->setBool("smooth", settings.shade == Shade::Smooth);
    shader->setInt("paint", settings.paint);

    graphics->device()->enableBias(true);

    shader->setVector("temp", Vec3(1, 1, 1));

    graphics->device()->renderTriangleList(buffers->previewBuffer()->buffer(), sizeof(BasicVertex));

    graphics->device()->enableBias(false);
}

void
ModelView::renderOrthographicGrid(VertexBuffer *buffer, const Matrix &viewMatrix, const Matrix &projMatrix)
{
    Matrix worldMatrix = identityMatrix();

    graphics->setPointVertexDeclaration();

    VertexShader *shader = graphics->setWireFrameVertexShader();
    shader->setMatrix("worldviewproj", worldMatrix * viewMatrix * projMatrix);

    graphics->device()->enableDepth(false);
    graphics->device()->enableDepthWrite(false);

    D3DCOLOR color = D3DCOLOR_XRGB(100, 100, 100);

    QPointF min = screenToZPlane(QPointF(0, 0));
    QPointF max = screenToZPlane(QPointF(size().width(), size().height()));

    float maxCells = 15.0f;

    float grid = 1.0f;
    float unit = orthoUnit(settings.pos, size(), fov);

    float cellPixels = unit * grid;

    float across = float(int(float(size().width()) / cellPixels) / 2) + 1.0f;
    float down = float(int(float(size().height()) / cellPixels) / 2) + 1.0f;

    while(across > maxCells || down > maxCells)
    {
        grid *= 2.0f;

        cellPixels = unit * grid;

        across = float(int(float(size().width()) / cellPixels) / 2) + 1.0f;
        down = float(int(float(size().height()) / cellPixels) / 2) + 1.0f;
    }

    Vec3 center = settings.pos;

    center.x -= fmod(center.x, grid);
    center.y -= fmod(center.y, grid);

    buffer->begin(D3DLOCK_DISCARD);

    for(float y = -down; y <= down; ++y)
    {
        if(buffer->count() + int(sizeof(PointVertex) * 2) > buffer->capacity())
        {
            buffer->end();
            graphics->device()->renderLineList(buffer, sizeof(PointVertex));

            buffer->begin(D3DLOCK_DISCARD);
        }

        buffer->add(PointVertex(Vec3(min.x(), center.y + (y * grid), 0), color));
        buffer->add(PointVertex(Vec3(max.x(), center.y + (y * grid), 0), color));
    }

    buffer->end();
    graphics->device()->renderLineList(buffer, sizeof(PointVertex));

    buffer->begin(D3DLOCK_DISCARD);

    for(float x = -across; x <= across; ++x)
    {
        if(buffer->count() + int(sizeof(PointVertex) * 2) > buffer->capacity())
        {
            buffer->end();
            graphics->device()->renderLineList(buffer, sizeof(PointVertex));

            buffer->begin(D3DLOCK_DISCARD);
        }

        buffer->add(PointVertex(Vec3(center.x + (x * grid), min.y(), 0), color));
        buffer->add(PointVertex(Vec3(center.x + (x * grid), max.y(), 0), color));
    }

    buffer->end();
    graphics->device()->renderLineList(buffer, sizeof(PointVertex));

    graphics->device()->enableDepth(true);
    graphics->device()->enableDepthWrite(true);
}

void
ModelView::renderJoint(VertexShader *shader, const Vec3 &pos, bool selected, const Matrix &worldMatrix, const Matrix &viewMatrix, const Matrix &projMatrix)
{
    shader->setMatrix("worldviewproj", translationMatrix(pos) * worldMatrix * viewMatrix * projMatrix);

    Buffer *buffer = selected ? buffers->selectedJointBuffer() : buffers->normalJointBuffer();
    graphics->device()->renderLineList(buffer->buffer(), sizeof(PointVertex));
}

void
ModelView::renderBone(VertexShader *shader, const Vec3 &a, const Vec3 &b, const QColor &color, const Matrix &worldMatrix, const Matrix &viewMatrix, const Matrix &projMatrix)
{
    float radius = buffers->jointRadius();

    VertexBuffer *buffer = graphics->temporaryVertexBuffer();

    D3DCOLOR c = toD3dColor(color);

    Vec3 n = normalizeVector(b - a);

    Vec3 ap = a + (n * radius);
    Vec3 bp = b - (n * radius);

    Vec3 m = Vec3(-n.y, n.x, 0);

    Vec3 p0 = normalizeVector(crossVectors(n, m)) * radius;
    Vec3 p1 = normalizeVector(crossVectors(n, p0)) * radius;

    QVector<Vec3> p = QVector<Vec3>() << ap + p1 << ap + p0 << ap - p1 << ap - p0;

    buffer->begin(D3DLOCK_DISCARD);

    for(int i = 0; i < 4; ++i)
    {
        buffer->add(PointVertex(p[i], c));
        buffer->add(PointVertex(bp, c));
    }

    for(int i = 0; i < 4; ++i)
    {
        buffer->add(PointVertex(p[i], c));
        buffer->add(PointVertex(p[i < 3 ? i + 1 : 0], c));
    }

    buffer->end();

    shader->setMatrix("worldviewproj", worldMatrix * viewMatrix * projMatrix);
    graphics->device()->renderLineList(buffer, sizeof(PointVertex));
}

void
ModelView::renderSkeleton(const Matrix &worldMatrix, const Matrix &viewMatrix, const Matrix &projMatrix)
{
    graphics->setPointVertexDeclaration();

    VertexShader *shader = graphics->setWireFrameVertexShader();

    graphics->device()->getDevice()->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

    for(int i = 0; i < model->jointCount(); ++i)
    {
        if(!model->joint(i).hide)
        {
            if(model->mode() == Mode::Animate)
            {
                Vec3 pos = model->transformedJointPosition(i);
                renderJoint(shader, pos, model->joint(i).selected, worldMatrix, viewMatrix, projMatrix);

                int p = model->joint(i).details.parent;
                if(p >= 0 && !model->joint(p).hide)
                {
                    renderBone(shader, model->transformedJointPosition(p), pos, Joint::color(), worldMatrix, viewMatrix, projMatrix);
                }
            }
            else
            {
                renderJoint(shader, model->joint(i).pos, model->joint(i).selected, worldMatrix, viewMatrix, projMatrix);

                int p = model->joint(i).details.parent;
                if(p >= 0 && !model->joint(p).hide)
                {
                    renderBone(shader, model->joint(p).pos, model->joint(i).pos, Joint::color(), worldMatrix, viewMatrix, projMatrix);
                }
            }
        }
    }

    graphics->device()->setVertexDeclaration();
    graphics->device()->setVertexShader();
}

void
ModelView::renderCursor(VertexBuffer *buffer, const Matrix &worldMatrix, const Matrix &viewMatrix, const Matrix &projMatrix)
{
    graphics->setPointVertexDeclaration();

    VertexShader *shader = graphics->setWireFrameVertexShader();

    shader->setMatrix("worldviewproj", worldMatrix * viewMatrix * projMatrix);

    graphics->device()->enableDepth(false);
    graphics->device()->enableDepthWrite(false);

    D3DCOLOR color = D3DCOLOR_XRGB(64, 130, 255);

    buffer->begin(D3DLOCK_DISCARD);

    buffer->add(PointVertex(model->cursor(), color));

    buffer->end();

    graphics->device()->setTexture(0, graphics->cursorTexture());
    graphics->device()->enableAlphaBlend(true);

    graphics->device()->setPointSize(32);
    graphics->device()->enablePointSpriteTexturing(true);

    graphics->device()->renderPointList(buffer, sizeof(PointVertex));

    graphics->device()->enablePointSpriteTexturing(false);

    graphics->device()->setTexture(0);
    graphics->device()->enableAlphaBlend(false);

    graphics->device()->setVertexDeclaration();
    graphics->device()->setVertexShader();

    graphics->device()->enableDepth(true);
    graphics->device()->enableDepthWrite(true);
}
