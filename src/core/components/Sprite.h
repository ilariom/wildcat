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
    Sprite(const std::string& filename) : ss1(filename), ss2(ss1) { setBuffersTo(&ss1, &ss2); }

public:
    bool unique() const override { return false; }
    wkt::math::Size size() const { return this->ss1.size(); }

private:
    wkt::gph::SmartSurface ss1;
    wkt::gph::SmartSurface ss2;
};

REGISTER_DRAWABLE(Sprite, -3);

}}

#endif