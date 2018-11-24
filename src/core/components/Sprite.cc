#include "Sprite.h"

namespace wkt {
namespace components
{

bool Sprite::unique() const
{
    return false;
}

void Sprite::draw(const wkt::gph::Director& director, const Transform& transform)
{
    director.shot(*this->outBuffer, transform);
}

void Sprite::shade(const pixel_manipulator& pixmanip)
{
    auto sz = size();
    std::swap(this->inBuffer, this->outBuffer);
    
    for(int x = 0; x < sz.width; ++x)
        for(int y = 0; y < sz.height; ++y)
        {
            wkt::gph::PixelIterator p = (*this->inBuffer)(x, y);
            wkt::gph::PixelIterator o = (*this->outBuffer)(x, y);
            *o = pixmanip(p);
        }
}

void Sprite::resetShading()
{
    this->ss1.resetSurface();
    this->ss2.resetSurface();
}

}}