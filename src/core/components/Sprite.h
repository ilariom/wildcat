#ifndef _WKT_SPRITE_H
#define _WKT_SPRITE_H

#include "ecs/Drawable.h"
#include "graphics/SmartSurface.h"
#include "graphics/Color.h"
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
    inline void setColor(const wkt::gph::Color&);
    inline void setOpacity(uint8_t);
    const wkt::gph::Color& getColor() const { return this->ss1.getColor(); }
    uint8_t getOpacity() const { return this->ss1.getOpacity(); }
    const std::string& getPath() const { return this->ss1.getPath(); }

private:
    wkt::gph::SmartSurface ss1;
    wkt::gph::SmartSurface ss2;
};

inline void Sprite::setColor(const wkt::gph::Color& color)
{
    this->ss1.setColor(color);
    this->ss2.setColor(color);
}

inline void Sprite::setOpacity(uint8_t opacity)
{
    this->ss1.setOpacity(opacity);
    this->ss2.setOpacity(opacity);
}

REGISTER_DRAWABLE(Sprite, -3);

}}

#endif