#ifndef _WKT_SMART_SURFACE_H
#define _WKT_SMART_SURFACE_H

#include "s2x/video.h"
#include "math/math.h"
#include "Color.h"
#include "Pixel.h"
#include <memory>
#include <string>

namespace wkt {
namespace gph
{

class SmartSurface
{
    friend Pixel;
    
public:
    explicit SmartSurface(const wkt::math::Size& size) : surface(std::make_shared<s2x::Surface>(size.width, size.height)) { }
    explicit SmartSurface(const std::string& filename);

public:
    inline Pixel operator()(int x, int y);
    inline const Pixel operator()(int x, int y) const;
    s2x::Texture& getTexture();
    wkt::math::Size size() const { return { this->surface->size().width, this->surface->size().height }; }

    explicit operator bool() const { return static_cast<SDL_Surface*>(*this->surface) != nullptr; }

private:
    void copyOnWrite();

private:
    std::shared_ptr<s2x::Surface> surface;
    std::shared_ptr<s2x::Texture> texture;
    bool surfaceModified = false;
    std::string filename;
};

inline Pixel SmartSurface::operator()(int x, int y)
{
    Pixel p(*this);
    s2x::Pixel* pp = &p;
    *pp = (*this->surface)(x, y);
    return p;
}

inline const Pixel SmartSurface::operator()(int x, int y) const
{
    return (*this)(x, y);
}

}}


#endif