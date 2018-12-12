#include "SurfaceStream.h"

namespace wkt {
namespace gph
{

SurfaceStream& SurfaceStream::operator<<(const wkt::gph::SmartSurface& ss)
{
    this->surfaces.emplace_back(&ss, this->positions.size() - 1);
    return *this;
}

SurfaceStream& SurfaceStream::operator<<(const wkt::math::vec2& v)
{
    this->positions.push_back(v);
    return *this;
}

wkt::gph::SmartSurface& SurfaceStream::sync()
{
    for(auto& p : this->surfaces)
    {
        wkt::math::vec2& position = this->positions[p.second];
        auto surface = p.first;

        this->target.blit(*surface, position);
    }

    return this->target;
}

}}