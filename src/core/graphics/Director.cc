#include "Director.h"

namespace wkt {
namespace gph
{

void Director::shot(SmartSurface& ss, const wkt::components::Transform& transform) const
{
    assert(this->camera != nullptr);

    auto wcoords = transform.getWorldCoordinates();
    auto scoords = this->camera->getScreenCoordinates(transform).getCoordinates();
    auto sz = ss.size();
    auto& texture = ss.getTexture();

    SDL_Rect r = { 
        (int)(scoords.position.x / wcoords.scaleX), 
        (int)(scoords.position.y / wcoords.scaleY), 
        (int)(sz.width), 
        (int)(sz.height)
    };

    SDL_Point ra = { 
        (int)scoords.rotationAnchor.x, 
        (int)scoords.rotationAnchor.y 
    };

    this->ren.copy(texture, r, scoords.rotation, ra, wcoords.scaleX, wcoords.scaleY);
}

}}