#ifndef MODELDATA_H
#define MODELDATA_H

#include <QtCore/QVector>
#include <QtCore/QString>

#include <QtGui/QColor>

#include "model/Vertex.h"
#include "model/Face.h"
#include "model/Joint.h"
#include "model/Animation.h"
#include "model/Group.h"

#include "dialogs/ExportDialog.h"

class Model;

class ModelData
{
public:
    ModelData(Model *model);

    bool
    isEmpty() const;

    void
    clear();

    bool
    loadFromFile(const QString &path);

    bool
    saveToFile(const QString &path) const;

    Model*
    model() const { return parent; }

    QVector<Vertex> vertices;
    QVector<Face> faces;
    QVector<Joint> joints;
    QVector<Animation> animations;
    QVector<Group> groups;
    QVector<QColor> palette;

    int hide;

    Vec3 cursor;

    ExportDetails lastExportDetails;

private:
    Model *parent;
};

#endif // MODELDATA_H
