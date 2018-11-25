#ifndef _WKT_DRAWABLE_H
#define _WKT_DRAWABLE_H

#include "ecs/Component.h"
#include "components/Transform.h"
#include "graphics/Color.h"
#include "graphics/Pixel.h"
#include "graphics/Director.h"
#include "graphics/SmartSurface.h"
#include "math/wktmath.h"
#include <memory>

namespace wkt {
namespace ecs
{

class Drawable : public Component
{
public:
    virtual void draw(const wkt::gph::Director&, const wkt::components::Transform&) = 0;
    void setVisible(bool enable) { this->visible = enable; }
    bool isVisible() const { return this->visible; }
    
private:
    bool visible = true;
};

class ShadedDrawable : public Drawable
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

template<typename T, typename... Args>
inline std::shared_ptr<Drawable> makeDrawable(Args... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

}}

#endif