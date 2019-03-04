#include "BoxMarquee.h"

#include "graphics/Graphics.h"
#include "graphics/GraphicsDevice.h"
#include "graphics/VertexShader.h"
#include "graphics/VertexBuffer.h"
#include "graphics/PointVertex.h"

#include "views/ModelView.h"

BoxMarquee::BoxMarquee(QObject *parent)
    : QObject(parent),
      activeView(0)
{
}

void
BoxMarquee::setAnchor(const QPoint &p)
{
    anc = p;
}

void
BoxMarquee::setPosition(const QPoint &p)
{
    pos = p;
}

void
BoxMarquee::setActiveView(ModelView *view)
{
    activeView = view;
}

QRectF
BoxMarquee::clipRect(const QSize &size) const
{
    float xa = float(anc.x());
    float ya = float(anc.y());

    float xb = float(pos.x());
    float yb = float(pos.y());

    if(xa > xb) qSwap(xa, xb);
    if(ya > yb) qSwap(ya, yb);

    const float dot = 1.0f;

    if(xa == xb)
    {
        xa -= dot;
        xb += dot;
    }

    if(ya == yb)
    {
        ya -= dot;
        yb += dot;
    }

    float hx = float(size.width()) / 2.0f;
    float hy = float(size.height()) / 2.0f;

    QRectF r;

    r.setTopLeft(QPointF((xa - hx) / hx, (ya -hy) / hy));
    r.setBottomRight(QPointF((xb - hx) / hx, (yb -hy) / hy));

    return r;
}

void
BoxMarquee::render(ModelView *view, Graphics *graphics)
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

    QPointF a = anc;
    QPointF p = pos;

    D3DCOLOR color = D3DCOLOR_ARGB(64, 50, 100, 255);

    buffer->begin(D3DLOCK_DISCARD);

    buffer->add(PointVertex(Vec3(a.x(), a.y(), 0), color));
    buffer->add(PointVertex(Vec3(p.x(), p.y(), 0), color));
    buffer->add(PointVertex(Vec3(a.x(), p.y(), 0), color));

    buffer->add(PointVertex(Vec3(a.x(), a.y(), 0), color));
    buffer->add(PointVertex(Vec3(p.x(), a.y(), 0), color));
    buffer->add(PointVertex(Vec3(p.x(), p.y(), 0), color));

    buffer->end();

    graphics->device()->setCullMode(D3DCULL_NONE);
    graphics->device()->enableAlphaBlend(true);

    graphics->device()->renderTriangleList(buffer, sizeof(PointVertex));

    graphics->device()->setCullMode(D3DCULL_CCW);
    graphics->device()->enableAlphaBlend(false);

    color = D3DCOLOR_XRGB(255, 255, 255);

    buffer->begin(D3DLOCK_DISCARD);

    buffer->add(PointVertex(Vec3(a.x(), a.y(), 0), color));
    buffer->add(PointVertex(Vec3(p.x(), a.y(), 0), color));

    buffer->add(PointVertex(Vec3(p.x(), a.y(), 0), color));
    buffer->add(PointVertex(Vec3(p.x(), p.y(), 0), color));

    buffer->add(PointVertex(Vec3(p.x(), p.y(), 0), color));
    buffer->add(PointVertex(Vec3(a.x(), p.y(), 0), color));

    buffer->add(PointVertex(Vec3(a.x(), p.y(), 0), color));
    buffer->add(PointVertex(Vec3(a.x(), a.y(), 0), color));

    buffer->end();

    graphics->device()->renderLineList(buffer, sizeof(PointVertex));

    graphics->device()->enableDepth(true);
    graphics->device()->enableDepthWrite(true);
}

