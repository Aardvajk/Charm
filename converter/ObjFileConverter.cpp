#include "ObjFileConverter.h"

#include "tiny_obj_loader.h"

#include "model/Model.h"

#include "command/CreateCommand.h"

ObjFileConverter::ObjFileConverter()
{
}

bool
ObjFileConverter::toModel(const QString &path, Model *model)
{
    QVector<Vertex> verts;
    QVector<Face> faces;

    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    QString message = tinyobj::LoadObj(shapes, materials, path.toStdString().c_str()).c_str();

    if(message != "")
    {
        err = message;
        return false;
    }

    if(shapes.size() == 0)
    {
        err = "No shapes found";
        return false;
    }

    for(size_t s = 0; s < shapes.size(); ++s)
    {
        tinyobj::shape_t &shape = shapes[s];

        size_t offset = verts.size();

        for(size_t i = 0; i < shape.mesh.positions.size(); i += 3)
        {
            Vec3 v;
            v.x = shape.mesh.positions[i];
            v.y = shape.mesh.positions[i + 1];
            v.z = shape.mesh.positions[i + 2];

            verts.push_back(Vertex(v, true));
        }

        for(size_t i = 0; i < shape.mesh.indices.size(); i += 3)
        {
            uint a = shape.mesh.indices[i] + offset;
            uint b = shape.mesh.indices[i + 1] + offset;
            uint c = shape.mesh.indices[i + 2] + offset;

            Face f(a, b, c);
            f.selected = true;

            faces.push_back(f);
        }
    }

    CreateCommand *command = new CreateCommand("Import", model, 0);

    command->setVertices(verts);
    command->setFaces(faces);

    command->activate(false);
    model->endCommand(command);

    return true;
}

bool
ObjFileConverter::fromModel(float scale, const Vec3 &offset, const QString &path, Model *model)
{
    return false;
}

