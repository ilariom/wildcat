#ifndef WKT_DIRECTOR_H
#define WKT_DIRECTOR_H

#include "s2x/video.h"
#include "graphics/SmartSurface.h"
#include "graphics/Camera.h"
#include "components/Transform.h"
#include "math/wktmath.h"

namespace wkt {
namespace gph
{

class Director final
{
public:
    Director(s2x::Renderer& ren) : ren(ren) { }

public:
    void shot(SmartSurface&, const wkt::components::Transform& transform) const;
    s2x::Renderer& getRenderer() const { return this->ren; }
    Camera* getCamera() const { return this->camera; }
    void setCamera(Camera* camera) { this->camera = camera; }

private:
    s2x::Renderer& ren;
    Camera* camera = nullptr;
};

}}

#endif