#ifndef MODELVIEWSETTINGS_H
#define MODELVIEWSETTINGS_H

#include "maths/Vec3.h"

#include "common/Projection.h"
#include "common/Rotation.h"
#include "common/Shade.h"
#include "common/Axis.h"
#include "common/Paint.h"

class Settings;

class ModelViewSettings
{
public:
    ModelViewSettings();

    void
    loadSettings(Settings *settings);

    void
    saveSettings(Settings *settings) const;

    Vec3 pos;
    Vec3 rotxy;

    Projection::Type proj;
    Rotation::Type rot;

    bool wireframe;

    Shade::Type shade;
    Axis::Type ref;
    bool smooth;
    bool skeleton;
    Paint::Type paint;
};

#endif // MODELVIEWSETTINGS_H
