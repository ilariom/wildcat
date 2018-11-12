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
    class Scheme_ref
    {
    public:
        Scheme_ref(const std::string& name) : name(name) { }

    public:
        Scheme_ref& operator+=(std::tuple<types...>* t) { relation.push_back(t); return *this; }
        size_t size() const { return this->relation.size(); }
        std::tuple<types...>& operator[](size_t k) { return *this->relation[k]; }

        const std::string& getName() const { return this->name; }

    private:
        std::vector<std::tuple<types...>*> relation;
        std::string name;
    };

public:
    friend class Scheme_ref;

public:
    Scheme(const std::string& name) : Table(name) { }
    Scheme(Scheme_ref&& sr) : Table(sr.getName())
    {
        for(int k = 0; k < sr.size(); ++k)
            this->relation.push_back(sr[k]);
    }
    Scheme(const Scheme&) = default;

    inline void update(types... vals);
    inline void remove(types... vals);

    template<int N>
    Scheme_ref selectWhere(typename std::tuple_element<N, std::tuple<types...>>::type value)
    {
        Scheme_ref sr(getName());

        for(auto& t : this->relation)
            if(std::get<N>(t) == value)
                sr += &t;

        return sr;
    }

private:
    std::vector<std::tuple<types...>> relation;
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
std::shared_ptr<Table> make_table_from_scheme(const Scheme<types...>& scheme)
{
    return std::make_shared<Table>(static_cast<Table>(scheme));
}

REGISTER_COMPONENT(Table, -7);

}}

#endif