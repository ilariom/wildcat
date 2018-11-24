#ifndef WKT_CACHE_BASE_H
#define WKT_CACHE_BASE_H

#include <string>
#include <memory>
#include <unordered_map>

namespace wkt {
namespace utils
{

template<typename T>
class CacheBase final
{
public:
    CacheBase(const CacheBase&) = delete;
    CacheBase(CacheBase&&) = delete;
    ~CacheBase() = default;

    CacheBase& operator=(const CacheBase&) = delete;
    CacheBase& operator=(CacheBase&&) = delete;
    
public:
    static inline CacheBase& getInstance();
    inline std::shared_ptr<T> operator[](const std::string&);

private:
    CacheBase() = default;

private:
    std::unordered_map<std::string, std::shared_ptr<T>> cache;
};

template<typename T>
inline CacheBase<T>& CacheBase<T>::getInstance()
{
    static CacheBase<T> sc;
    return sc;
}

template<typename T>
inline std::shared_ptr<T> CacheBase<T>::operator[](const std::string& filename)
{
    if(this->cache.find(filename) == this->cache.end())
    {
        this->cache[filename] = std::make_shared<T>(filename);
    }

    return this->cache[filename];
}

}}

#endif