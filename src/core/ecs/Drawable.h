#ifndef _WKT_DRAWABLE_H
#define _WKT_DRAWABLE_H

#include "ecs/Component.h"
#include "components/Transform.h"
#include "graphics/Color.h"
#include "graphics/Pixel.h"
#include "math/wktmath.h"
#include "graphics/Director.h"
#include <memory>

namespace wkt {
namespace ecs
{

class Drawable : public Component
{
public:
    virtual void draw(const wkt::gph::Director&, const wkt::components::Transform&) = 0;
};

class ShadedDrawable : public Drawable
{
public:
    using pixel_manipulator = std::function<wkt::gph::Color(const wkt::gph::PixelIterator&)>;

public:
    virtual void shade(const pixel_manipulator&) { }
    virtual void resetShading() { }
};

template<typename T, typename... Args>
inline std::shared_ptr<Drawable> makeDrawable(Args... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

}}

#endif