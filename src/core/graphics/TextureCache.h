#ifndef _WKT_TEXTURE_CACHE_H
#define _WKT_TEXTURE_CACHE_H

#include "s2x/video.h"
#include <string>
#include <memory>
#include <unordered_map>

namespace wkt {
namespace gph
{

class TextureCache final
{
public:
    TextureCache(const TextureCache&) = delete;
    TextureCache(TextureCache&&) = delete;
    ~TextureCache() = default;

    TextureCache& operator=(const TextureCache&) = delete;
    TextureCache& operator=(TextureCache&&) = delete;

public:
    static inline TextureCache& getInstance();
    inline std::shared_ptr<s2x::Texture> at(const std::string&);
    inline void insert(const std::string& filename, std::shared_ptr<s2x::Texture> tex);
    inline bool find(const std::string&);

    void setRenderer(s2x::Renderer* renderer) { this->ren = renderer; }
    s2x::Renderer* renderer() const { return this->ren; }

private:
    TextureCache() = default;

private:
    std::unordered_map<std::string, std::shared_ptr<s2x::Texture>> cache;
    s2x::Renderer* ren;
};

inline TextureCache& TextureCache::getInstance()
{
    static TextureCache tc;
    return tc;
}

inline std::shared_ptr<s2x::Texture> TextureCache::at(const std::string& filename)
{
    if(this->cache.find(filename) == this->cache.end())
    {
        return nullptr;
    }

    return this->cache[filename];
}

inline void TextureCache::insert(const std::string& filename, std::shared_ptr<s2x::Texture> tex)
{
    this->cache[filename] = tex;
}

inline bool TextureCache::find(const std::string& filename)
{
    return this->cache.find(filename) != this->cache.end();
}

}}

#endif