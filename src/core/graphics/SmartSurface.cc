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
    this->commonSurface = surfaceCache[this->filename];
    this->activeSurface = this->commonSurface.get();

    auto& texCache = TextureCache::getInstance();

    if(!texCache.find(this->filename))
        texCache.insert(this->filename, std::make_shared<s2x::Texture>(*texCache.renderer(), *this->commonSurface));

    this->texture = texCache.at(this->filename);

    assert(*this);
}

SmartSurface::SmartSurface(const SmartSurface& ss)
{
    this->filename = ss.filename;
    this->commonSurface = ss.commonSurface;
    
    if(ss.localSurface)
    {
        this->localSurface = std::make_unique<s2x::Surface>(s2x::Surface(*ss.localSurface.get()));
        this->activeSurface = this->localSurface.get();
        this->isAlreadyCloned = true;
    }
    else
    {
        this->activeSurface = this->commonSurface.get();
        this->isAlreadyCloned = false;
    }

    this->surfaceModified = false;
}

s2x::Texture& SmartSurface::getTexture()
{
    if(!this->isAlreadyCloned || !this->surfaceModified)
        return *this->texture;

    *this->texture = this->activeSurface == this->commonSurface.get() ? *this->commonSurface : *this->localSurface;
    return *this->texture;
}

void SmartSurface::copyOnAccess()
{
    this->surfaceModified = true;

    if(this->isAlreadyCloned)
        return;
    
    this->localSurface = std::make_unique<s2x::Surface>(s2x::Surface(*this->commonSurface));
    this->activeSurface = this->localSurface.get();
    this->texture = std::make_shared<s2x::Texture>(*TextureCache::getInstance().renderer(), *this->localSurface);
    this->isAlreadyCloned = true;
}

void SmartSurface::resetSurface()
{
    this->activeSurface = this->commonSurface.get();
    this->texture = TextureCache::getInstance().at(this->filename);
    this->isAlreadyCloned = false;
    this->surfaceModified = false;
    this->localSurface = nullptr;
}

}}