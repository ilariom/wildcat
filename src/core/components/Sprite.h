#ifndef _WKT_SPRITE_H
#define _WKT_SPRITE_H

#include "ecs/Drawable.h"
#include "graphics/SmartSurface.h"
#include "math/math.h"
#include <string>

namespace wkt {
namespace components
{

class Sprite : public wkt::ecs::Drawable
{
public:
    Sprite(const std::string& filename) : ss(filename) { }

public:
    void draw(s2x::Renderer&, const Transform&) override;
    wkt::math::Size size() const { return this->ss.size(); }

private:
    wkt::gph::SmartSurface ss;
};

REGISTER_DRAWABLE(Sprite, -3);

}}

#endif