#ifndef WKT_SURFACE_STREAM_H
#define WKT_SURFACE_STREAM_H

#include "SmartSurface.h"
#include "math/wktmath.h"
#include <vector>

namespace wkt::gph
{

class SurfaceStream final
{
public:
    SurfaceStream(int width, int height) : target(s2x::Surface(width, height)) { this->positions.emplace_back(); }
    SurfaceStream(const SurfaceStream&) = delete;
    SurfaceStream(SurfaceStream&&) = delete;
    ~SurfaceStream() = default;

    SurfaceStream& operator=(const SurfaceStream&) = delete;
    SurfaceStream& operator=(SurfaceStream&&) = delete;

public:
    SurfaceStream& operator<<(const wkt::gph::SmartSurface&);
    SurfaceStream& operator<<(const wkt::math::vec2&);

    wkt::gph::SmartSurface& sync();
    wkt::gph::SmartSurface& read() { return this->target; }
    const wkt::gph::SmartSurface& read() const { return this->target; }

private:
    wkt::gph::SmartSurface target;
    std::vector<std::pair<const wkt::gph::SmartSurface*, size_t>> surfaces;
    std::vector<wkt::math::vec2> positions;
};

}

#endif