#ifndef _WKT_COLOR_H
#define _WKT_COLOR_H

#include "s2x/video.h"
#include "math/numerical.h"
#include <ostream>

namespace wkt {
namespace gph
{

class Color
{
public:
    Color(float r, float g, float b, float a)
        : r(r), g(g), b(b), a(a)
    { }
    inline Color(const s2x::Color& sdlCol);

    Color() = default;

    operator s2x::Color() const
    {
        constexpr float factor = 255.f;
        s2x::Color c;
        c.r = (Uint8)(factor * this->r);
        c.g = (Uint8)(factor * this->g);
        c.b = (Uint8)(factor * this->b);
        c.a = (Uint8)(factor * this->a);

        return c;
    }

public:
    inline Color& operator+=(const Color& other);
    inline Color& operator-=(const Color& other);
    inline Color& operator*=(float s);
    inline Color& operator/=(float s);

public:
    float r = 0, g = 0, b = 0, a = 0;
};

inline Color::Color(const s2x::Color& sdlCol) : r(sdlCol.r), g(sdlCol.g), b(sdlCol.b), a(sdlCol.a)
{
    *this /= 256.f;
}

inline Color& Color::operator+=(const Color& other)
{
    this->r = wkt::math::clampNorm(this->r + other.r);
    this->g = wkt::math::clampNorm(this->g + other.g);
    this->b = wkt::math::clampNorm(this->b + other.b);
    this->a = wkt::math::clampNorm(this->a + other.a);

    return *this;
}

inline Color& Color::operator-=(const Color& other)
{
    this->r = wkt::math::clampNorm(this->r - other.r);
    this->g = wkt::math::clampNorm(this->g - other.g);
    this->b = wkt::math::clampNorm(this->b - other.b);
    this->a = wkt::math::clampNorm(this->a - other.a);

    return *this;
}

inline Color& Color::operator*=(float s)
{
    this->r = wkt::math::clampNorm(this->r * s);
    this->g = wkt::math::clampNorm(this->g * s);
    this->b = wkt::math::clampNorm(this->b * s);
    this->a = wkt::math::clampNorm(this->a * s);

    return *this;
}

inline Color& Color::operator/=(float s)
{
    this->r = wkt::math::clampNorm(this->r / s);
    this->g = wkt::math::clampNorm(this->g / s);
    this->b = wkt::math::clampNorm(this->b / s);
    this->a = wkt::math::clampNorm(this->a / s);

    return *this;
}

inline Color operator+(const Color& a, const Color& b)
{
    return Color(a) += b;
}

inline Color operator-(const Color& a, const Color& b)
{
    return Color(a) -= b;
}

inline Color operator*(const Color& a, float s)
{
    return Color(a) *= s;
}

inline Color operator*(float s, const Color& a)
{
    return a * s;
}

inline std::ostream& operator<<(std::ostream& os, const Color& c)
{
    os << "(" << c.r << ", " << c.g << ", " << c.b << ", " << c.a << ")";
    return os;
}

namespace colors
{
    extern const Color RED;
    extern const Color GREEN;
    extern const Color BLUE;
    extern const Color WHITE;
    extern const Color BLACK;
}

}}

#endif