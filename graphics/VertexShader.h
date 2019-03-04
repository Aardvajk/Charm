#ifndef VERTEXSHADER_H
#define VERTEXSHADER_H

#include <QtCore/QObject>
#include <QtCore/QMap>

#include <d3dx9.h>
#include <d3d9.h>

class GraphicsDevice;

class VertexShader : public QObject
{
    Q_OBJECT

public:
    VertexShader(GraphicsDevice *graphics, QObject *parent = 0);
    ~VertexShader();

    bool
    acquire(const QString &file);

    void
    release();

    void
    setDefaults();

    void
    setMatrix(const QString &name, const D3DXMATRIX &matrix);

    void
    setVector(const QString &name, const D3DXVECTOR4 &vector);

    void
    setVector(const QString &name, const D3DXVECTOR3 &vector);

    void
    setInt(const QString &name, int value);

    void
    setFloat(const QString &name, float value);

    void
    setBool(const QString &name, bool value);

    void
    setMatrixArray(const QString &name, const D3DXMATRIX *values, size_t count);

    IDirect3DVertexShader9*
    getVertexShader() const { return ptr; }

private:
    D3DXHANDLE
    handle(const QString &name);

    GraphicsDevice *graphics;
    IDirect3DVertexShader9 *ptr;
    ID3DXConstantTable *table;
    QMap<QString, D3DXHANDLE> handles;
};

#endif // VERTEXSHADER_H
