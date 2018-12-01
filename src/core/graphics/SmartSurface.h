#ifndef _WKT_SMART_SURFACE_H
#define _WKT_SMART_SURFACE_H

#include "s2x/video.h"
#include "math/wktmath.h"
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
    SmartSurface(const std::string& filename, const wkt::math::Rect& crop = {});
    SmartSurface(const s2x::Surface&);
    SmartSurface(const SmartSurface&);
    SmartSurface(SmartSurface&&) = default;
    ~SmartSurface() = default;
 
    SmartSurface& operator=(const SmartSurface&);
    SmartSurface& operator=(SmartSurface&&) = default;

public:
    inline PixelIterator operator()(int x, int y);
    inline const PixelIterator operator()(int x, int y) const;
    s2x::Texture& getTexture();
    inline wkt::math::Size size() const;
    void resetSurface();
    const std::string& getPath() const { return this->filename; }

    void setColor(const Color& color) { this->color = color; }
    const Color& getColor() const { return this->color; }
    void setOpacity(uint8_t opacity) { this->opacity = opacity; }
    uint8_t getOpacity() const { return this->opacity; }

    void crop(const wkt::math::Rect& rect) { this->texRect = rect; }
    const wkt::math::Rect& getTextureRect() const { return this->texRect; }

    explicit operator bool() const { return static_cast<SDL_Surface*>(*this->activeSurface) != nullptr; }

private:
    void copyOnAccess();

private:
    std::shared_ptr<s2x::Surface> commonSurface;
    std::shared_ptr<s2x::Texture> commonTexture;
    std::unique_ptr<s2x::Surface> localSurface = nullptr;
    std::unique_ptr<s2x::Texture> localTexture = nullptr;
    s2x::Surface* activeSurface;
    bool surfaceModified = false;
    bool isAlreadyCloned = false;
    std::string filename;
    Color color = colors::WHITE;
    uint8_t opacity = 255;
    wkt::math::Rect texRect;
};

inline PixelIterator SmartSurface::operator()(int x, int y)
{
    copyOnAccess();
    PixelIterator p(*this, {(float)x, (float)y});
    s2x::Pixel* pp = reinterpret_cast<s2x::Pixel*>(&p);
    *pp = (*this->activeSurface)(x, y);
    return p;
}

inline const PixelIterator SmartSurface::operator()(int x, int y) const
{
    PixelIterator p(const_cast<SmartSurface&>(*this), {(float)x, (float)y});
    s2x::Pixel* pp = &p;
    *pp = (*this->activeSurface)(x, y);
    return p;
}

inline wkt::math::Size SmartSurface::size() const
{
    if(this->texRect.size.width > 0 && this->texRect.size.height > 0)
        return this->texRect.size;

    return this->activeSurface->size();
}

}}


#endif