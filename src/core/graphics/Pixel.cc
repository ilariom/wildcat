#include "Pixel.h"
#include "SmartSurface.h"

namespace wkt {
namespace gph
{

PixelIterator::PixelIterator(SmartSurface& ss, wkt::math::vec2&& pos)
    : s2x::Pixel(ss.activeSurface->pixelFormat()), ss(ss), pos(std::move(pos))
{ }

void PixelIterator::set(const Color& c)
{
    s2x::Pixel::set(c.r * 255, c.g * 255, c.b * 255, c.a * 255);
}

Color PixelIterator::get() const
{
    return Color(s2x::Pixel::asRGBA());
}

PixelIterator PixelIterator::operator+(const wkt::math::vec2& offset)
{
    return PixelIterator(this->ss, this->pos + offset);
}

const PixelIterator PixelIterator::operator+(const wkt::math::vec2& offset) const
{
    return PixelIterator(this->ss, this->pos + offset);
}

PixelIterator::ColorRef PixelIterator::operator*()
{
    return ColorRef(*this);
}

const PixelIterator::ColorRef PixelIterator::operator*() const
{
    return ColorRef(*this);
}

const wkt::math::Size PixelIterator::size() const
{ 
    return this->ss.size(); 
}

}}