#ifndef _WKT_TABLE_H
#define  _WKT_TABLE_H

#include "ecs/Component.h"
#include <string>
#include <tuple>
#include <vector>
#include <algorithm>
#include <memory>

namespace wkt {
namespace components
{

class Table : public wkt::ecs::Component
{
public:
    Table(const std::string& name) : name(name) { }

public:
    bool unique() const override { return false; }
    const std::string& getName() const { return this->name; }

private:
    std::string name;
};

template<typename... types>
class Scheme : public Table
{
public:
    Scheme(const std::string& name) : Table(name) { }
    inline Scheme(const Scheme&);
    Scheme(Scheme&&) = default;

    inline Scheme& operator=(const Scheme&);
    Scheme& operator=(Scheme&&) = default;
public:
    inline void update(types... vals);
    inline void remove(types... vals);

    Scheme& operator+=(const std::tuple<types...>& t) { this->relation.push_back(t); return *this; }
    inline std::tuple<types...>& operator[](size_t n);

    template<int N>
    Scheme selectWhere(typename std::tuple_element<N, std::tuple<types...>>::type value)
    {
        Scheme sr(getName());
        sr.schemeRef = this->schemeRef == nullptr ? this : this->schemeRef;

        for(auto& t : this->relation)
            if(std::get<N>(t) == value)
                sr += t;

        return sr;
    }

private:
    std::vector<std::tuple<types...>> relation;
    Scheme<types...>* schemeRef = nullptr;
};

template<typename... types>
inline void Scheme<types...>::update(types... vals)
{
    auto t = std::make_tuple<types...>(std::forward<types>(vals)...);
    this->relation.push_back(t);
}

template<typename... types>
inline void Scheme<types...>::remove(types... vals)
{
    auto t = std::make_tuple<types...>(std::forward<types>(vals)...);
    auto it = std::find(this->relation.begin(), this->relation.end(), t);

    if(it != this->relation.end())
        this->relation.erase(it);
}

template<typename... types>
inline Scheme<types...>::Scheme(const Scheme& sch)
{
    this->relation = sch.relation;
    this->schemeRef = nullptr;
}

template<typename... types>
inline Scheme<types...>& Scheme<types...>::operator=(const Scheme& sch)
{
    this->relation = sch.relation;
    this->schemeRef = nullptr;
    return *this;
}

template<typename... types>
inline std::tuple<types...>& Scheme<types...>::operator[](size_t n)
{
    if(this->schemeRef)
    {
        auto t = this->relation[n];

        for(auto& v : this->schemeRef->relation)
            if(v == t)
                return v;
    }

    return this->relation[n];
}

template<typename... types>
std::shared_ptr<Table> make_table_from_scheme(const Scheme<types...>& scheme)
{
    return std::make_shared<Table>(static_cast<Table>(scheme));
}

REGISTER_COMPONENT(Table, -7);

}}

#endif