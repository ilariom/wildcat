#ifndef _WKT_PIXEL_H
#define _WKT_PIXEL_H

#include "s2x/video.h"
#include "math/wktmath.h"
#include "Color.h"

namespace wkt {
namespace gph
{

class SmartSurface;

class PixelIterator final : public s2x::Pixel
{
public:
    class ColorRef final : public Color
    {
        friend PixelIterator;
        
    public:
        explicit ColorRef(const PixelIterator& pi) : pi(pi) { *this += pi.get(); }

    public:
        ColorRef& operator=(const Color& c)
        {
            static_cast<Color>(*this) = c;
            const_cast<PixelIterator&>(pi).set(c);
            return *this;
        }

    private:
        const PixelIterator& pi;
    };

public:
    PixelIterator(SmartSurface&, wkt::math::vec2&&);
    PixelIterator(const PixelIterator&) = default;
    PixelIterator(PixelIterator&&) = default;

public:
    const PixelIterator operator+(const wkt::math::vec2& offset) const;
    ColorRef operator*();
    const ColorRef operator*() const;
    const wkt::math::vec2& position() const { return this->pos; }
    const wkt::math::Size size() const;

private:
    void set(const Color& c);
    Color get() const;
    
private:
    SmartSurface& ss;
    wkt::math::vec2 pos;
};

}}

#endif