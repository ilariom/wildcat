#ifndef WKT_SHADED_DRAWABLE
#define WKT_SHADED_DRAWABLE

#include "ecs/Drawable.h"
#include "graphics/Color.h"
#include "graphics/Pixel.h"
#include "graphics/SmartSurface.h"

namespace wkt {
namespace gph
{

class ShadedDrawable : public wkt::ecs::Drawable
{
public:
    using pixel_manipulator = std::function<wkt::gph::Color(const wkt::gph::PixelIterator&)>;

public:
    void draw(const wkt::gph::Director&, const wkt::components::Transform&);
    virtual void shade(const pixel_manipulator&);
    virtual void resetShading();

protected:
    void setBuffersTo(wkt::gph::SmartSurface* ss1, wkt::gph::SmartSurface* ss2);

private:
    wkt::gph::SmartSurface* inBuffer = nullptr;
    wkt::gph::SmartSurface* outBuffer = nullptr;
};

}}

#endif