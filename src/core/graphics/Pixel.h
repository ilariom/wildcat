#ifndef _WKT_PIXEL_H
#define _WKT_PIXEL_H

#include "s2x/video.h"
#include "Color.h"

namespace wkt {
namespace gph
{

class SmartSurface;

class Pixel : public s2x::Pixel
{
public:
    explicit Pixel(SmartSurface&);
    Pixel(const Pixel&) = default;
    Pixel(Pixel&&) = default;

public:
    void set(const Color& c);
    Color get() const;
    
private:
    SmartSurface& ss;
};


}}

#endif