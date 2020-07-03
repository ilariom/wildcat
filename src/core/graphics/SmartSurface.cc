#include "SmartSurface.h"
#include "SurfaceCache.h"
#include "TextureCache.h"
#include "utils/search_path.h"
#include <cassert>

namespace wkt {
namespace gph
{

SmartSurface::SmartSurface(const std::string& filename, const wkt::math::Rect& crop)
{
    this->filename = wkt::path::locate(filename);
    auto& surfaceCache = SurfaceCache::getInstance();
    this->commonSurface = surfaceCache[this->filename];
    this->activeSurface = this->commonSurface.get();

    auto& texCache = TextureCache::getInstance();

    if(!texCache.find(this->filename))
        texCache.insert(this->filename, std::make_shared<s2x::Texture>(*texCache.renderer(), *this->commonSurface));

    this->commonTexture = texCache.at(this->filename);
    this->texRect = crop;

    assert(*this);
}

SmartSurface::SmartSurface(const s2x::Surface& srf)
{
    this->commonSurface = std::make_shared<s2x::Surface>(srf);
    this->activeSurface = this->commonSurface.get();
    auto& texCache = TextureCache::getInstance();
    this->commonTexture = std::make_shared<s2x::Texture>(*texCache.renderer(), *this->commonSurface);
    this->texRect.size = {
        (float) srf.size().width,
        (float) srf.size().height
    };
}

SmartSurface::SmartSurface(const SmartSurface& ss)
{
    operator=(ss);
}

SmartSurface& SmartSurface::operator=(const SmartSurface& ss)
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
    this->texRect = ss.texRect;

    return *this;
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
    
    if(this->texRect.size.width != 0 && this->texRect.size.height != 0)
        this->localSurface = std::make_unique<s2x::Surface>(s2x::Surface(*this->commonSurface, this->texRect));
    else
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

void SmartSurface::blit(const SmartSurface& other, const wkt::math::vec2& position)
{
    copyOnAccess();

    SDL_Rect r = {
        (int)position.x,
        (int)position.y,
        (int)other.size().width,
        (int)other.size().height
    };

    this->activeSurface->blit(r, *other.activeSurface);
}

void SmartSurface::blit(const SmartSurface& other, const wkt::math::vec2& position, float scaleX, float scaleY)
{
    copyOnAccess();

    SDL_Rect r = {
        (int)position.x,
        (int)position.y,
        (int)other.size().width,
        (int)other.size().height
    };

    this->activeSurface->blit(r, *other.activeSurface, scaleX, scaleY);
}

}}