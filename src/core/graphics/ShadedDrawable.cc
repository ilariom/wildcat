#include "ShadedDrawable.h"

namespace wkt {
namespace gph 
{

void ShadedDrawable::setBuffersTo(wkt::gph::SmartSurface* ss1, wkt::gph::SmartSurface* ss2)
{
    this->inBuffer = ss1;
    this->outBuffer = ss2;
}

void ShadedDrawable::draw(const wkt::gph::Director& director, const wkt::components::Transform& transform)
{
    director.shot(*this->outBuffer, transform);
}

void ShadedDrawable::shade(const pixel_manipulator& pixmanip)
{
    auto sz = this->inBuffer->size();
    std::swap(this->inBuffer, this->outBuffer);
    
    for(int x = 0; x < sz.width; ++x)
        for(int y = 0; y < sz.height; ++y)
        {
            wkt::gph::PixelIterator p = (*this->inBuffer)(x, y);
            wkt::gph::PixelIterator o = (*this->outBuffer)(x, y);
            *o = pixmanip(p);
        }
}

void ShadedDrawable::resetShading()
{
    this->inBuffer->resetSurface();
    this->outBuffer->resetSurface();
}

}}