#include "VertexShader.h"
#include "GraphicsDevice.h"

#include <QtCore/QFile>
#include <QtCore/QByteArray>

VertexShader::VertexShader(GraphicsDevice *graphics, QObject *parent)
    : QObject(parent),
      graphics(graphics),
      ptr(0),
      table(0)
{
}

VertexShader::~VertexShader()
{
    release();
}

bool
VertexShader::acquire(const QString &file)
{
    QFile input(file);
    if(!input.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    QByteArray src = input.readAll();

    ID3DXBuffer *buffer = 0, *errors = 0;

    HRESULT r = D3DXCompileShader(QString::fromLatin1(src).toStdString().c_str(), src.length(), 0, 0, "main", "vs_3_0", 0, &buffer, &errors, &table);

    if(errors)
    {
        errors->Release();
        return false;
    }

    if(FAILED(r)) return false;

    r = graphics->getDevice()->CreateVertexShader(reinterpret_cast<DWORD*>(buffer->GetBufferPointer()), &ptr);

    if(FAILED(r))
    {
        buffer->Release();
        table->Release();

        ptr = 0;
        table = 0;

        return false;
    }

    buffer->Release();
    table->SetDefaults(graphics->getDevice());

    return true;
}

void
VertexShader::release()
{
    if(table)
    {
        table->Release();
        table = 0;
    }

    if(ptr)
    {
        ptr->Release();
        ptr = 0;
    }
}

void
VertexShader::setDefaults()
{
    table->SetDefaults(graphics->getDevice());
}

void
VertexShader::setMatrix(const QString &name, const D3DXMATRIX &matrix)
{
    table->SetMatrix(graphics->getDevice(), handle(name), &matrix);
}

void
VertexShader::setVector(const QString &name, const D3DXVECTOR4 &vector)
{
    table->SetVector(graphics->getDevice(), handle(name), &vector);
}

void
VertexShader::setVector(const QString &name, const D3DXVECTOR3 &vector)
{
    setVector(name, D3DXVECTOR4(vector.x, vector.y, vector.z, 1.0f));
}

void
VertexShader::setInt(const QString &name, int value)
{
    table->SetInt(graphics->getDevice(), handle(name), value);
}

void
VertexShader::setFloat(const QString &name, float value)
{
    table->SetFloat(graphics->getDevice(), handle(name), value);
}

void
VertexShader::setBool(const QString &name, bool value)
{
    table->SetBool(graphics->getDevice(), handle(name), value);
}

void
VertexShader::setMatrixArray(const QString &name, const D3DXMATRIX *values, size_t count)
{
    table->SetMatrixArray(graphics->getDevice(), handle(name), values, count);
}

D3DXHANDLE
VertexShader::handle(const QString &name)
{
    if(!handles.contains(name))
    {
        handles.insert(name, table->GetConstantByName(0, name.toStdString().c_str()));
    }

    return handles[name];
}
