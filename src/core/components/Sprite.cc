#include "Sprite.h"

namespace wkt {
namespace components
{

void Sprite::draw(s2x::Renderer& renderer, const Transform& transform)
{
    auto& texture = ss.getTexture();

    auto coords = transform.getWorldCoordinates();
    auto size = this->size();

    SDL_Rect r = { 
        coords.position.x / coords.scaleX, 
        coords.position.y / coords.scaleY, 
        size.width, 
        size.height 
    };

    SDL_Point ra = { 
        coords.rotationAnchor.x, 
        coords.rotationAnchor.y 
    };

    renderer.copy(texture, r, coords.rotation, ra, coords.scaleX, coords.scaleY);
}

}}