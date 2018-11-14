#include "SmartSurface.h"
#include "SurfaceCache.h"
#include "TextureCache.h"
#include "globals/Director.h"
#include <cassert>

namespace wkt {
namespace gph
{

SmartSurface::SmartSurface(const std::string& filename)
{
    this->filename = "../res/" + filename;
    auto& surfaceCache = SurfaceCache::getInstance();
    this->surface = surfaceCache[this->filename];

    assert(*this);
}

s2x::Texture& SmartSurface::getTexture()
{
    auto& texCache = TextureCache::getInstance();
    this->texture = texCache.at(this->filename);
    bool fromCache = this->texture != nullptr;

    if(this->surfaceModified)
    {
        if(fromCache)
        {
            s2x::Renderer* renderer = texCache.renderer();
            this->texture = std::make_shared<s2x::Texture>(*renderer, *(this->surface));
        }
        else
        {
            *(this->texture) = *(this->surface);
        }

        this->surfaceModified = false;
    }
    else if(this->texture == nullptr)
    {
        s2x::Renderer* renderer = texCache.renderer();
        this->texture = std::make_shared<s2x::Texture>(*renderer, *(this->surface));
        texCache.insert(this->filename, this->texture);
    }

    return *(this->texture);
}

void SmartSurface::copyOnWrite()
{
    if(this->isAlreadyCloned)
        return;
    
    this->surfaceModified = true;
    this->surface = std::make_shared<s2x::Surface>(*(this->surface));
    this->isAlreadyCloned = true;
}

}}