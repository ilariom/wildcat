#include "SmartSurface.h"
#include "SurfaceCache.h"
#include "TextureCache.h"
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

    this->commonTexture = texCache.at(this->filename);

    assert(*this);
}

SmartSurface::SmartSurface(const s2x::Surface& srf)
{
    this->commonSurface = std::make_shared<s2x::Surface>(srf);
    this->activeSurface = this->commonSurface.get();
    auto& texCache = TextureCache::getInstance();
    this->commonTexture = std::make_shared<s2x::Texture>(*texCache.renderer(), *this->commonSurface);
}

SmartSurface::SmartSurface(const SmartSurface& ss)
{
    this->filename = ss.filename;
    this->commonSurface = ss.commonSurface;
    this->commonTexture = ss.commonTexture;
    
    if(ss.localSurface)
    {
        this->localSurface = std::make_unique<s2x::Surface>(s2x::Surface(*ss.localSurface.get()));
        this->activeSurface = this->localSurface.get();
        this->localTexture = std::make_unique<s2x::Texture>(*TextureCache::getInstance().renderer(), *ss.localSurface);
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
    if(!this->isAlreadyCloned || this->activeSurface == this->commonSurface.get())
    {
        this->commonTexture->setColor(this->color);
        this->commonTexture->setOpacity(this->opacity);
        return *this->commonTexture;
    }

    if(this->surfaceModified)
    {
        *this->localTexture = *this->localSurface;
        this->surfaceModified = false;
    }

    this->localTexture->setColor(this->color);
    this->localTexture->setOpacity(this->opacity);
    return *this->localTexture;
}

void SmartSurface::copyOnAccess()
{
    this->surfaceModified = true;

    if(this->isAlreadyCloned)
        return;
    
    this->localSurface = std::make_unique<s2x::Surface>(s2x::Surface(*this->commonSurface));
    this->activeSurface = this->localSurface.get();
    this->localTexture = std::make_unique<s2x::Texture>(*TextureCache::getInstance().renderer(), *this->localSurface);
    this->isAlreadyCloned = true;
}

void SmartSurface::resetSurface()
{
    this->activeSurface = this->commonSurface.get();
    this->isAlreadyCloned = false;
    this->surfaceModified = false;
    this->localSurface = nullptr;
}

}}