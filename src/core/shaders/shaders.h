#ifndef WKT_SHADERS_H
#define WKT_SHADERS_H

#include "math/math.h"
#include "math/numerical.h"
#include "graphics/Color.h"

namespace wkt {
namespace shaders
{

class lighten
{
public:
    lighten(float factor) : factor(wkt::math::clamp(factor, 0, 1)) { }

    inline wkt::gph::Color operator()(const wkt::gph::Color& c, const wkt::math::vec2&, const wkt::math::Size&)
    {
        float m = std::max({c.r, c.g, c.b});
        float a = 1 / m;
        return { 
            std::max(this->factor, a * c.r), 
            std::max(this->factor, a * c.g), 
            std::max(this->factor, a * c.b), 
            c.a
        };
    }

private:
    float factor;
};

class darken
{
public:
    darken(float factor) : factor(wkt::math::clamp(factor, 0, 1)) { }

    inline wkt::gph::Color operator()(const wkt::gph::Color& c, const wkt::math::vec2&, const wkt::math::Size&)
    {
        float m = std::max({c.r, c.g, c.b});
        float a = 1 - m;
        return { 
            std::min(this->factor, a * c.r), 
            std::min(this->factor, a * c.g), 
            std::min(this->factor, a * c.b), 
            c.a
        };
    }

private:
    float factor;
};

class blackAndWhite
{
public:
    blackAndWhite(float br = 1, float bg = 1, float bb = 1)
    {
        this->den = br + bg + bb;
        this->br = br / this->den;
        this->bg = bg / this->den;
        this->bb = bb / this->den;
    }

    inline wkt::gph::Color operator()(const wkt::gph::Color& c, const wkt::math::vec2&, const wkt::math::Size&)
    {
        float v = (this->br * c.r + this->bg * c.g + this->bb * c.b) / this->den;
        return { 
            v, v, v, c.a
        };
    }

private:
    float br, bg, bb, den;
};

}}

#endif