#include "Pixel.h"
#include "SmartSurface.h"

namespace wkt {
namespace gph
{

Pixel::Pixel(SmartSurface& ss)
    : s2x::Pixel(ss.surface->pixelFormat()), ss(ss)
{ }

void Pixel::set(const Color& c)
{
    this->ss.surfaceModified = true;
    s2x::Pixel::set(c.r * 255, c.g * 255, c.b * 255, c.a * 255);
}

Color Pixel::get() const
{
    return Color(s2x::Pixel::asRGBA());
}

}}