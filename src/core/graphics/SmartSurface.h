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
    friend PixelIterator;
    
public:
    explicit SmartSurface(const std::string& filename);

    SmartSurface(const SmartSurface&);

public:
    inline PixelIterator operator()(int x, int y);
    inline const PixelIterator operator()(int x, int y) const;
    s2x::Texture& getTexture();
    wkt::math::Size size() const { return { this->activeSurface->size().width, this->activeSurface->size().height }; }
    void resetSurface();

    explicit operator bool() const { return static_cast<SDL_Surface*>(*this->activeSurface) != nullptr; }

private:
    void copyOnAccess();

private:
    std::shared_ptr<s2x::Surface> commonSurface;
    std::unique_ptr<s2x::Surface> localSurface = nullptr;
    s2x::Surface* activeSurface;
    std::shared_ptr<s2x::Texture> texture;
    bool surfaceModified = false;
    bool isAlreadyCloned = false;
    std::string filename;
};

inline PixelIterator SmartSurface::operator()(int x, int y)
{
    copyOnAccess();
    PixelIterator p(*this, {x, y});
    s2x::Pixel* pp = reinterpret_cast<s2x::Pixel*>(&p);
    *pp = (*this->activeSurface)(x, y);
    return p;
}

inline const PixelIterator SmartSurface::operator()(int x, int y) const
{
    PixelIterator p(const_cast<SmartSurface&>(*this), {x, y});
    s2x::Pixel* pp = &p;
    *pp = (*this->activeSurface)(x, y);
    return p;
}

}}


#endif