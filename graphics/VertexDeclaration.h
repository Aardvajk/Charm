#ifndef VERTEXDECLARATION_H
#define VERTEXDECLARATION_H

#include <QtCore/QObject>
#include <QtCore/QVector>

#include <d3d9.h>

class GraphicsDevice;

class VertexElement
{
public:
    VertexElement();
    VertexElement(WORD stream, WORD offset, BYTE type, BYTE usage, BYTE index);

private:
    D3DVERTEXELEMENT9 val;
};

class VertexDeclaration : public QObject
{
    Q_OBJECT

public:
    VertexDeclaration(GraphicsDevice *graphics, QObject *parent = 0);
    ~VertexDeclaration();

    bool
    acquire(const QVector<VertexElement> &elements);

    void
    release();

    IDirect3DVertexDeclaration9*
    getDeclaration() const { return ptr; }

private:
    GraphicsDevice *graphics;
    IDirect3DVertexDeclaration9 *ptr;
};


#endif // VERTEXDECLARATION_H
