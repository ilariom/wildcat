#ifndef _WKT_DRAWABLE_H
#define _WKT_DRAWABLE_H

#include "ecs/Component.h"
#include "components/Transform.h"
#include "graphics/Color.h"
#include "math/math.h"
#include "s2x/video.h"
#include <memory>

namespace wkt {
namespace ecs
{

class Drawable : public Component
{
public:
    using soft_shader = std::function<wkt::gph::Color(const wkt::gph::Color&, const wkt::math::vec2&, const wkt::math::Size&)>;

public:
    virtual void draw(s2x::Renderer&, const wkt::components::Transform&) = 0;
    virtual void shade(const soft_shader&) { }
    virtual void resetShading() { }
};

template<typename T, typename... Args>
inline std::shared_ptr<Drawable> makeDrawable(Args... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

}}

#endif