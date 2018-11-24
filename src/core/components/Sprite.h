#ifndef _WKT_SPRITE_H
#define _WKT_SPRITE_H

#include "ecs/Drawable.h"
#include "graphics/SmartSurface.h"
#include "math/wktmath.h"
#include <string>

namespace wkt {
namespace components
{

class Sprite : public wkt::ecs::ShadedDrawable
{
public:
    Sprite(const std::string& filename) : ss1(filename), ss2(ss1) { }

public:
    bool unique() const override;
    void draw(const wkt::gph::Director&, const Transform&) override;
    void shade(const pixel_manipulator&) override;
    void resetShading() override;
    wkt::math::Size size() const { return this->outBuffer->size(); }

private:
    wkt::gph::SmartSurface ss1;
    wkt::gph::SmartSurface ss2;
    wkt::gph::SmartSurface* inBuffer = &this->ss1;
    wkt::gph::SmartSurface* outBuffer = &this->ss2;
};

REGISTER_DRAWABLE(Sprite, -3);

}}

#endif