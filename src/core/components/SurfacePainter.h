#ifndef WKT_SURFACE_PAINTER_H
#define WKT_SURFACE_PAINTER_H

#include "ecs/Drawable.h"
#include "graphics/SmartSurface.h"

namespace wkt::components
{

class SurfacePainter : public wkt::ecs::Drawable
{
    using SmartSurface = wkt::gph::SmartSurface;

public:
    explicit SurfacePainter(const SmartSurface& ss) : ss(ss) { }
    explicit SurfacePainter(SmartSurface&& ss) : ss(std::move(ss)) { }

public:
    void draw(const wkt::gph::Director& d, const wkt::components::Transform& t) override
    {
        d.shot(this->ss, t);
    }

    bool unique() const override { return false; }
    SmartSurface& getSurface() { return this->ss; }

private:
    SmartSurface ss;
};

REGISTER_DRAWABLE(SurfacePainter, -15);

}

#endif