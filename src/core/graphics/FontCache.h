#ifndef _WKT_FONT_CACHE
#define _WKT_FONT_CACHE

#include "s2x/ttf.h"
#include <string>
#include <memory>
#include <unordered_map>

namespace wkt {
namespace gph
{

class FontCache final
{
public:
    FontCache(const FontCache&) = delete;
    FontCache(FontCache&&) = delete;
    ~FontCache() = default;

    FontCache& operator=(const FontCache&) = delete;
    FontCache& operator=(FontCache&&) = delete;
    
public:
    static inline FontCache& getInstance();
    inline std::shared_ptr<s2x::TrueTypeFont> operator()(const std::string&, int);

private:
    FontCache() = default;

private:
    std::unordered_map<std::string, std::shared_ptr<s2x::TrueTypeFont>> cache;
};

inline FontCache& FontCache::getInstance()
{
    static FontCache sc;
    return sc;
}

inline std::shared_ptr<s2x::TrueTypeFont> FontCache::operator()(const std::string& filename, int fontSize)
{
    std::string key = filename + std::to_string(fontSize);

    if(this->cache.find(key) == this->cache.end())
    {
        this->cache[key] = std::make_shared<s2x::TrueTypeFont>(filename, fontSize);
    }

    return this->cache[key];
}

}}

#endif