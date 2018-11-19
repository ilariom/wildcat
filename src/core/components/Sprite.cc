#include "Sprite.h"

namespace wkt {
namespace components
{

bool Sprite::unique() const
{
    return false;
}

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

void Sprite::shade(const soft_shader& softShader)
{
    auto sz = size();

    for(int x = 0; x < sz.width; ++x)
        for(int y = 0; y < sz.height; ++y)
        {
            wkt::gph::PixelIterator p = this->ss(x, y);
            *p = softShader(p);
        }
}

void Sprite::resetShading()
{
    this->ss.resetSurface();
}

}}