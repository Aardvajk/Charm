#include "GraphicsDevice.h"
#include "GraphicsWidget.h"

#include <QtWidgets/QWidget>

#include <QtCore/QEvent>

#include <stdexcept>
#include <cstdlib>

#include "graphics/VertexDeclaration.h"
#include "graphics/VertexShader.h"
#include "graphics/VertexBuffer.h"
#include "graphics/Texture.h"

namespace
{

D3DPRESENT_PARAMETERS
createParams(HWND hw, const QSize &size)
{
    D3DPRESENT_PARAMETERS p;
    ZeroMemory(&p, sizeof(D3DPRESENT_PARAMETERS));

    p.BackBufferWidth = (size.width() > 0 ? size.width() : 1);
    p.BackBufferHeight = (size.height() > 0 ? size.height() : 1);

    p.BackBufferFormat = D3DFMT_A8R8G8B8;
    p.BackBufferCount = 1;

    p.MultiSampleType = D3DMULTISAMPLE_NONE;

    p.SwapEffect = D3DSWAPEFFECT_DISCARD;
    p.hDeviceWindow = hw;
    p.Windowed = true;
    p.EnableAutoDepthStencil = false;
    p.AutoDepthStencilFormat = D3DFMT_D24S8;
    p.Flags = 0;

    p.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    p.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

    return p;
}

void
setGlobalDeviceSettings(IDirect3DDevice9 *device)
{
    DWORD filter = D3DTEXF_LINEAR;

    device->SetSamplerState(0, D3DSAMP_MAGFILTER, filter);
    device->SetSamplerState(0, D3DSAMP_MINFILTER, filter);

    device->SetSamplerState(0, D3DSAMP_MIPFILTER, filter);

    device->SetSamplerState(1, D3DSAMP_MAGFILTER, filter);
    device->SetSamplerState(1, D3DSAMP_MINFILTER, filter);

    device->SetSamplerState(1, D3DSAMP_MIPFILTER, filter);

    device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

    device->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    device->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

    device->SetRenderState(D3DRS_ZENABLE, true);
    device->SetRenderState(D3DRS_LIGHTING, false);

    device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
    device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
}

}

class SwapChain
{
public:
    SwapChain() : ptr(0), depthStencilPtr(0) { }
    ~SwapChain(){ release(); }

    void
    release();

    IDirect3DSwapChain9 *ptr;
    IDirect3DSurface9 *depthStencilPtr;
};

void
SwapChain::release()
{
    if(ptr)
    {
        ptr->Release();
        ptr = 0;
    }

    if(depthStencilPtr)
    {
        depthStencilPtr->Release();
        depthStencilPtr = 0;
    }
}

GraphicsDevice::GraphicsDevice(QObject *parent)
    : QObject(parent),
      direct3d(0),
      device(0),
      backBuffer(0),
      requiresReset(false)
{
}

GraphicsDevice::~GraphicsDevice()
{
    releaseDevice();

    if(direct3d)
    {
        direct3d->Release();
    }
}

bool
GraphicsDevice::initialise()
{
    direct3d = Direct3DCreate9(D3D_SDK_VERSION);
    return (direct3d != 0);
}

void
GraphicsDevice::scheduleReset()
{
    requiresReset = true;
}

void
GraphicsDevice::registerWidget(GraphicsWidget *widget)
{
    if(!widgets.contains(widget))
    {
        widgets[widget] = new SwapChain();

        widget->installEventFilter(this);

        requiresReset = true;
    }
}

void
GraphicsDevice::unregisterWidget(GraphicsWidget *widget)
{
    if(widgets.contains(widget))
    {
        delete widgets[widget];
        widgets.remove(widget);

        widget->removeEventFilter(this);

        requiresReset = true;
    }
}

void
GraphicsDevice::begin(GraphicsWidget *widget, D3DCOLOR clearColor)
{
    HRESULT r = widgets[widget]->ptr->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
    if(FAILED(r)) return;

    r = device->SetRenderTarget(0, backBuffer);
    if(FAILED(r)) return;

    r = device->SetDepthStencilSurface(widgets[widget]->depthStencilPtr);
    if(FAILED(r)) return;

    device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clearColor, 1.0f, 0);
    device->BeginScene();
}

void
GraphicsDevice::end(GraphicsWidget *widget)
{
    device->EndScene();
    widgets[widget]->ptr->Present(NULL, NULL, reinterpret_cast<HWND>(widget->winId()), NULL, 0);

    device->SetDepthStencilSurface(NULL);

    backBuffer->Release();
    backBuffer = 0;
}

void
GraphicsDevice::setVertexDeclaration(const VertexDeclaration *dec)
{
    device->SetVertexDeclaration(dec ? dec->getDeclaration() : 0);
}

void
GraphicsDevice::setVertexShader(const VertexShader *shader)
{
    device->SetVertexShader(shader ? shader->getVertexShader() : 0);
}

void
GraphicsDevice::setTexture(int stage, const Texture *texture)
{
    device->SetTexture(stage, texture ? texture->getTexture() : 0);
}

void
GraphicsDevice::setCullMode(DWORD mode)
{
    device->SetRenderState(D3DRS_CULLMODE, mode);
}

void
GraphicsDevice::enableBias(bool state)
{
    if(state)
    {
        float slope = 1.0f;
        device->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, *((DWORD*)(&slope)));

        float bias = 0.00000001f;
        device->SetRenderState(D3DRS_DEPTHBIAS, *((DWORD*)(&bias)));
    }
    else
    {
        float zero = 0.0f;
        device->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, *((DWORD*)(&zero)));
        device->SetRenderState(D3DRS_DEPTHBIAS, *((DWORD*)(&zero)));
    }
}

void
GraphicsDevice::enableDepth(bool state)
{
    device->SetRenderState(D3DRS_ZENABLE, state);
}

void
GraphicsDevice::enableDepthWrite(bool state)
{
    device->SetRenderState(D3DRS_ZWRITEENABLE, state);
}

void
GraphicsDevice::enableBufferWrite(bool state)
{
    if(state)
    {
        device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
        device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    }
    else
    {
        device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
        device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
        device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    }
}

void
GraphicsDevice::enableAlphaBlend(bool state)
{
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, state);
}

void
GraphicsDevice::enablePointSpriteTexturing(bool state)
{
    device->SetRenderState(D3DRS_POINTSPRITEENABLE, state);
}

void
GraphicsDevice::setPointSize(float size)
{
    device->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)(&size)));
}

void
GraphicsDevice::renderTriangleList(const VertexBuffer *buffer, size_t stride)
{
    if(buffer->count() / stride < 3) return;

    device->SetStreamSource(0, buffer->getVertexBuffer(), 0, stride);
    device->SetIndices(0);

    device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, (buffer->count() / stride) / 3);
}

void
GraphicsDevice::renderTriangleListGroup(const VertexBuffer *buffer, size_t stride, int start, int count)
{
    if(start < 0 || start + count > static_cast<int>(buffer->count() / stride)) return;

    device->SetStreamSource(0, buffer->getVertexBuffer(), 0, stride);
    device->SetIndices(0);

    device->DrawPrimitive(D3DPT_TRIANGLELIST, start * 3, count);
}

void
GraphicsDevice::renderLineList(const VertexBuffer *buffer, size_t stride)
{
    if(buffer->count() < 2) return;

    device->SetStreamSource(0, buffer->getVertexBuffer(), 0, stride);
    device->SetIndices(0);

    device->DrawPrimitive(D3DPT_LINELIST, 0, ((buffer->count() / stride) / 2));
}

void
GraphicsDevice::renderPointList(const VertexBuffer *buffer, size_t stride)
{
    device->SetStreamSource(0, buffer->getVertexBuffer(), 0, stride);
    device->SetIndices(0);

    device->DrawPrimitive(D3DPT_POINTLIST, 0, buffer->count() / stride);
}

bool
GraphicsDevice::isLost() const
{
    if(!device) return true;
    return device->TestCooperativeLevel() == D3DERR_DEVICELOST;
}

bool
GraphicsDevice::needsResetting() const
{
    return requiresReset;
}

bool
GraphicsDevice::isReadyToReset() const
{
    if(!device) return true;

    if(device->TestCooperativeLevel() == D3D_OK)
    {
        return true;
    }

    return device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET;
}

bool
GraphicsDevice::reset()
{
    if(widgets.empty())
    {
        releaseDevice();
        requiresReset = false;

        return true;
    }

    foreach(GraphicsWidget *widget, widgets.keys())
    {
        widgets[widget]->release();
    }

    QList<GraphicsWidget*> widgetList;
    foreach(GraphicsWidget *widget, widgets.keys())
    {
        widgetList.append(widget);
    }

    if(!device)
    {
        D3DCAPS9 caps;
        HRESULT r = direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
        if(FAILED(r)) return false;

        DWORD vertexProcessing;

        if(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
        else                                              vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

        D3DPRESENT_PARAMETERS params = createParams(reinterpret_cast<HWND>(widgetList.front()->winId()), widgetList.front()->size());

        r = direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, NULL, vertexProcessing, &params, &device);
        if(FAILED(r)) return false;

        setGlobalDeviceSettings(device);
    }
    else
    {
        D3DPRESENT_PARAMETERS params = createParams(reinterpret_cast<HWND>(widgetList[0]->winId()), widgetList[0]->size());

        HRESULT r;

        r = device->Reset(&params);
        if(FAILED(r))
        {
            releaseDevice();
            return false;
        }

        setGlobalDeviceSettings(device);
    }

    HRESULT r = device->GetSwapChain(0, &(widgets[widgetList[0]]->ptr));
    if(FAILED(r))
    {
        releaseDevice();
        return false;
    }

    r = device->CreateDepthStencilSurface(widgetList[0]->size().width(), widgetList[0]->size().height(), D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, false, &(widgets[widgetList[0]]->depthStencilPtr), NULL);
    if(FAILED(r))
    {
        releaseDevice();
        return false;
    }

    for(int i = 1; i < widgetList.size(); ++i)
    {
        D3DPRESENT_PARAMETERS params = createParams(reinterpret_cast<HWND>(widgetList[i]->winId()), widgetList[i]->size());

        r = device->CreateAdditionalSwapChain(&params, &(widgets[widgetList[i]]->ptr));
        if(FAILED(r))
        {
            releaseDevice();
            return false;
        }

        r = device->CreateDepthStencilSurface(widgetList[i]->size().width(), widgetList[i]->size().height(), D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, false, &(widgets[widgetList[i]]->depthStencilPtr), NULL);
        if(FAILED(r))
        {
            releaseDevice();
            return false;
        }
    }

    requiresReset = false;
    return true;
}

IDirect3DDevice9*
GraphicsDevice::getDevice() const
{
    return device;
}

bool
GraphicsDevice::eventFilter(QObject *object, QEvent *event)
{
    if(dynamic_cast<GraphicsWidget*>(object) && event->type() == QEvent::Resize)
    {
        requiresReset = true;
    }

    return QObject::eventFilter(object, event);
}

void
GraphicsDevice::releaseDevice()
{
    foreach(GraphicsWidget *widget, widgets.keys())
    {
        delete widgets[widget];
    }

    if(device)
    {
        device->Release();
        device = 0;
    }
}
