#ifndef _WKT_MULTISET_H
#define _WKT_MULTISET_H

#include <unordered_map>

namespace wkt {
namespace utils
{

template<typename T, typename Hasher = std::hash<T>>
class Multiset
{
public:
    class iterator
    {
    public:
        iterator(typename std::unordered_map<T, int, Hasher>::iterator it) : it(it) { }

    public:
        T& operator*() { return this->it.first; }
        iterator& operator++() { ++it; return it.first; }
        bool operator==(const iterator& i) { return this->it == i.it; }

    private:
        typename std::unordered_map<T, int, Hasher>::iterator it;
    };

public:
    inline void insert(T&& elem);
    inline void erase(T&& elem);
    inline bool has(T&& elem);

    iterator begin() { return iterator(this->data.begin()); }
    iterator end() { return iterator(this->data.end()); }

private:
    std::unordered_map<T, int, Hasher> data;
};

template<typename T, typename H>
inline void Multiset<T, H>::insert(T&& elem)
{
    if(this->data.find(elem) == this->data.end())
    {
        this->data[elem] = 0;
    }

    this->data[elem]++;
}

template<typename T, typename H>
inline void Multiset<T, H>::erase(T&& elem)
{
    if(this->data.find(elem) == this->data.end())
        return;

    this->data[elem]--;

    if(this->data[elem] <= 0)
        this->data.erase(elem);
}

template<typename T, typename H>
inline bool Multiset<T, H>::has(T&& elem)
{
    return this->data.find(elem) != this->data.end();
}

}}

#endif