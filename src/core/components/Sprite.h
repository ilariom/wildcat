#ifndef _WKT_SPRITE_H
#define _WKT_SPRITE_H

#include "graphics/ShadedDrawable.h"
#include "graphics/SmartSurface.h"
#include "graphics/Color.h"
#include "math/wktmath.h"
#include <string>

namespace wkt {
namespace components
{

class Sprite : public wkt::gph::ShadedDrawable
{
public:
    Sprite(const std::string& filename) : ss1(filename), ss2(ss1) { setBuffersTo(&ss1, &ss2); }
    inline Sprite(const Sprite&);
    inline Sprite(Sprite&&);
    ~Sprite() = default;

    inline Sprite& operator=(const Sprite&);
    inline Sprite& operator=(Sprite&&);

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

inline Sprite::Sprite(const Sprite& s) : ss1(s.ss1), ss2(s.ss2)
{
    setBuffersTo(&this->ss1, &this->ss2);
}

inline Sprite::Sprite(Sprite&& s) : ss1(std::move(s.ss1)), ss2(std::move(s.ss2))
{
    setBuffersTo(&this->ss1, &this->ss2);
}

inline Sprite& Sprite::operator=(const Sprite& s)
{
    this->ss1 = wkt::gph::SmartSurface(s.ss1);
    this->ss2 = wkt::gph::SmartSurface(s.ss2);
    setBuffersTo(&this->ss1, &this->ss2);

    return *this;
}

inline Sprite& Sprite::operator=(Sprite&& s)
{
    std::swap(this->ss1, s.ss1);
    std::swap(this->ss2, s.ss2);
    setBuffersTo(&this->ss1, &this->ss2);

    return *this;
}

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