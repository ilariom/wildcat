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

    auto cameraBB = this->camera->getBoundingBox();

    wkt::math::Rect objBB = {
        scoords.position,
        sz
    };

    if (!cameraBB.intersect(objBB))
        return;

    SDL_Rect r {
        (int) scoords.position.x,
        (int) scoords.position.y,
        (int) (sz.width * wcoords.scaleX),
        (int) (sz.height * wcoords.scaleY)
    };

    SDL_Point ra = { 
        (int) (scoords.rotationAnchor.x * r.w), 
        (int) (scoords.rotationAnchor.y * r.h)
    };

    const auto& texRect = ss.getTextureRect();

    if (texRect.size.width > 0 && texRect.size.height > 0)
        this->ren.copy(ss.getTexture(), texRect, r, scoords.rotation, ra);
    else
        this->ren.copy(ss.getTexture(), r, scoords.rotation, ra);
}

}}