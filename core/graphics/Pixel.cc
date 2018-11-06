#include "Pixel.h"
#include "SmartSurface.h"

namespace wkt {
namespace gph
{

Pixel::Pixel(SmartSurface& ss)
    : s2x::Pixel(static_cast<SDL_Surface*>(ss.surface.operator*())->format), ss(ss)
{ }

void Pixel::set(const Color& c)
{
    this->ss.copyOnWrite();
    s2x::Pixel::set(c.r, c.g, c.b, c.a);
}

}}