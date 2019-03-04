#include "LineHandle.h"

#include "maths/Maths.h"

#include "graphics/Graphics.h"
#include "graphics/GraphicsDevice.h"
#include "graphics/VertexShader.h"
#include "graphics/VertexBuffer.h"
#include "graphics/PointVertex.h"

#include "views/ModelView.h"

LineHandle::LineHandle(QObject *parent)
    : QObject(parent),
      activeView(0)
{
}

void
LineHandle::setAnchor(const QPoint &p)
{
    anc = p;
}

void
LineHandle::setPosition(const QPoint &p)
{
    pos = p;
}

void
LineHandle::setActiveView(ModelView *view)
{
    activeView = view;
}

void
LineHandle::render(ModelView *view, Graphics *graphics)
{
    if(activeView != view)
    {
        return;
    }

    graphics->setPointVertexDeclaration();

    VertexShader *shader = graphics->setScreenVertexShader();

    shader->setFloat("halfwidth", float(view->size().width()) / 2.0f);
    shader->setFloat("halfheight", float(view->size().height()) / 2.0f);

    graphics->device()->enableDepth(false);
    graphics->device()->enableDepthWrite(false);

    VertexBuffer *buffer = graphics->temporaryVertexBuffer();

    Vec3 a(anc.x(), anc.y(), 0);
    Vec3 p(pos.x(), pos.y(), 0);

    D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255);

    float radius = 7.0f;

    QVector<Vec3> circle;
    for(float a = 0; a < pi() * 2.0f; a += pi() / 10)
    {
        circle.append(Vec3(sinf(a) * radius, cosf(a) * radius, 0));
    }

    buffer->begin(D3DLOCK_DISCARD);

    for(int i = 0; i < circle.count(); ++i)
    {
        int j = i < circle.count() - 1 ? i + 1 : 0;

        buffer->add(PointVertex(a + circle[i], color));
        buffer->add(PointVertex(a + circle[j], color));
    }

    for(int i = 0; i < circle.count(); ++i)
    {
        int j = i < circle.count() - 1 ? i + 1 : 0;

        buffer->add(PointVertex(p + circle[i], color));
        buffer->add(PointVertex(p + circle[j], color));
    }

    if(vectorLength(a - p) > radius * 2.0f)
    {
        Vec3 n = normalizeVector(p - a);

        buffer->add(PointVertex(a + (n * radius), color));
        buffer->add(PointVertex(p - (n * radius), color));
    }

    buffer->end();

    graphics->device()->renderLineList(buffer, sizeof(PointVertex));

    graphics->device()->enableDepth(true);
    graphics->device()->enableDepthWrite(true);
}

