#ifndef WKT_SURFACE_CACHE_H
#define WKT_SURFACE_CACHE_H

#include "s2x/video.h"
#include "utils/CacheBase.h"
#include <string>
#include <memory>
#include <unordered_map>

namespace wkt {
namespace gph
{

using SurfaceCache = wkt::utils::CacheBase<s2x::Surface>;

// class SurfaceCache final
// {
// public:
//     SurfaceCache(const SurfaceCache&) = delete;
//     SurfaceCache(SurfaceCache&&) = delete;
//     ~SurfaceCache() = default;

//     SurfaceCache& operator=(const SurfaceCache&) = delete;
//     SurfaceCache& operator=(SurfaceCache&&) = delete;
    
// public:
//     static inline SurfaceCache& getInstance();
//     inline std::shared_ptr<s2x::Surface> operator[](const std::string&);

// private:
//     SurfaceCache() = default;

// private:
//     std::unordered_map<std::string, std::shared_ptr<s2x::Surface>> cache;
// };

// inline SurfaceCache& SurfaceCache::getInstance()
// {
//     static SurfaceCache sc;
//     return sc;
// }

// inline std::shared_ptr<s2x::Surface> SurfaceCache::operator[](const std::string& filename)
// {
//     if(this->cache.find(filename) == this->cache.end())
//     {
//         this->cache[filename] = std::make_shared<s2x::Surface>(filename);
//     }

//     return this->cache[filename];
// }

}}

#endif