#include "XFileConverter.h"

#include "graphics/Graphics.h"
#include "graphics/GraphicsDevice.h"

#include "model/Model.h"

#include "command/CreateCommand.h"

XFileConverter::XFileConverter(Graphics *graphics)
    : graphics(graphics),
      mesh(0),
      adjBuffer(0),
      materialBuffer(0)
{
}

bool
XFileConverter::toModel(const QString &path, Model *model)
{
    DWORD numberMaterials = 0;

    HRESULT hr = D3DXLoadMeshFromX(path.toStdString().c_str(), D3DXMESH_SYSTEMMEM, graphics->device()->getDevice(), &adjBuffer, &materialBuffer, 0, &numberMaterials, &mesh);
    if(FAILED(hr))
    {
        err = "D3DXLoadMeshFromX failed";
        return release();
    }

    D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];

    hr = mesh->GetDeclaration(decl);
    if(FAILED(hr))
    {
        err = "ID3DXBaseMesh::GetDeclaration failed";
        return release();
    }

    UINT vertSize = D3DXGetDeclVertexSize(decl, 0);

    WORD posOffset = -1;
    for(unsigned int i = 0; i < D3DXGetDeclLength(decl) ; ++i)
    {
        if(decl[i].Usage == D3DDECLUSAGE_POSITION)
        {
            posOffset = decl[i].Offset;
            break;
        }
    }

    if(posOffset < 0)
    {
        err = "Unable to find position data in vertex format";
        return release();
    }

    DWORD vertCount = mesh->GetNumVertices();

    VOID *ptr;
    hr = mesh->LockVertexBuffer(D3DLOCK_READONLY, &ptr);
    if(FAILED(hr))
    {
        err = "Unable to lock mesh vertex buffer";
        return release();
    }

    const char *data = static_cast<const char*>(ptr);

    QVector<Vertex> verts;

    for(DWORD i = 0; i < vertCount; ++i)
    {
        const char *vert = data + (i * vertSize);
        const char *pos = vert + posOffset;

        Vec3 v = *(reinterpret_cast<const Vec3*>(pos));

        verts.append(Vertex(v, true));
    }

    mesh->UnlockVertexBuffer();

    IDirect3DIndexBuffer9 *indexBuffer = 0;
    hr = mesh->GetIndexBuffer(&indexBuffer);
    if(FAILED(hr))
    {
        err = "Unable to get index buffer";
        return release();
    }

    D3DINDEXBUFFER_DESC desc;
    indexBuffer->GetDesc(&desc);

    indexBuffer->Release();

    hr = mesh->LockIndexBuffer(D3DLOCK_READONLY, &ptr);
    if(FAILED(hr))
    {
        err = "Unable to lock index buffer";
        return release();
    }

    data = static_cast<const char*>(ptr);

    DWORD faceCount = mesh->GetNumFaces();

    QVector<Face> faces;
    for(DWORD i = 0; i < faceCount; ++i)
    {
        if(desc.Format == D3DFMT_INDEX16)
        {
            const USHORT *n = reinterpret_cast<const USHORT*>(data);

            Face face(n[0], n[1], n[2]);
            faces.append(face);

            data += sizeof(USHORT) * 3;
        }
        else if(desc.Format == D3DFMT_INDEX32)
        {
            const UINT *n = reinterpret_cast<const UINT*>(data);

            Face face(n[0], n[1], n[2]);
            faces.append(face);

            data += sizeof(UINT) * 3;
        }
    }

    release();

    CreateCommand *command = new CreateCommand("Import", model, 0);

    command->setVertices(verts);
    command->setFaces(faces);

    command->activate(false);
    model->endCommand(command);

    return true;
}

bool
XFileConverter::fromModel(float scale, const QString &path, Model *model)
{
    return false;
}

bool
XFileConverter::release()
{
    if(materialBuffer)
    {
        materialBuffer->Release();
    }

    if(adjBuffer)
    {
        adjBuffer->Release();
    }

    if(mesh)
    {
        mesh->Release();
    }

    return false;
}

