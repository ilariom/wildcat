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

    SDL_Rect r = { 
        (int)(scoords.position.x / wcoords.scaleX), 
        (int)(scoords.position.y / wcoords.scaleY), 
        (int)(sz.width), 
        (int)(sz.height)
    };

    auto cameraBB = this->camera->getBoundingBox();
    wkt::math::Rect objBB = {
        scoords.position,
        sz
    };

    if(cameraBB.intersect(objBB))
    {
        auto &texture = ss.getTexture();
        auto texRect = ss.getTextureRect();

        SDL_Point ra = { 
            (int)(scoords.rotationAnchor.x * sz.width), 
            (int)(scoords.rotationAnchor.y * sz.height)
        };

        this->ren.copy(texture, r, scoords.rotation, ra, wcoords.scaleX, wcoords.scaleY, texRect);
    }
}

}}