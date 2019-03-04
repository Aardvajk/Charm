#include "ModelViewSettings.h"

#include "settings/Settings.h"

ModelViewSettings::ModelViewSettings()
    : pos(0, 0, -60),
      rotxy(0, 0, 0),
      proj(Projection::Perspective),
      rot(Rotation::Front),
      wireframe(true),
      shade(Shade::Smooth),
      ref(Axis::None),
      smooth(false),
      skeleton(true),
      paint(Paint::Palette)
{
}

void
ModelViewSettings::loadSettings(Settings *settings)
{
    proj = static_cast<Projection::Type>(settings->child("projection")->value().toInt());
    rot = static_cast<Rotation::Type>(settings->child("rotation")->value().toInt());
    wireframe = settings->child("wireframe")->value().toBool();
    shade = static_cast<Shade::Type>(settings->child("shade")->value().toInt());
    ref = static_cast<Axis::Type>(settings->child("reflection")->value().toInt());
    smooth = settings->child("smooth")->value().toBool();
    skeleton = settings->child("skeleton")->value().toBool();
    paint = Paint::Palette;
}

void
ModelViewSettings::saveSettings(Settings *settings) const
{
    settings->add("projection", proj);
    settings->add("rotation", rot);
    settings->add("wireframe", wireframe);
    settings->add("shade", shade);
    settings->add("reflection", ref);
    settings->add("smooth", smooth);
    settings->add("skeleton", skeleton);
    settings->add("paint", paint);
}
