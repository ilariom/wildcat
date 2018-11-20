#ifndef _WKT_COMPONENTS_VECTOR_H
#define _WKT_COMPONENTS_VECTOR_H

#include "ecs/Component.h"
#include <vector>
#include <memory>

namespace wkt {
namespace ecs
{

template<typename C = Component>
class ComponentsVector final : public std::vector<std::shared_ptr<Component>>
{
public:
    std::shared_ptr<C> get(size_type n) { return std::static_pointer_cast<C>(this->operator[](n)); }
    std::shared_ptr<C> first() { return get(0); }

    explicit operator bool() const { return !empty(); }
    std::shared_ptr<C> operator*() { return first(); }

    template<typename D = Component>
    ComponentsVector<C>& operator=(const ComponentsVector<D>& other)
    {
        using vect = std::vector<std::shared_ptr<Component>>;
        *static_cast<vect*>(this) = *static_cast<const vect*>(&other);
        return *this;
    }

    template<typename D = Component>
    ComponentsVector<C>& operator=(ComponentsVector<D>&& other)
    {
        using vect = std::vector<std::shared_ptr<Component>>;
        *static_cast<vect*>(this) = std::move(*static_cast<const vect*>(&other));
        return *this;
    }
};

}}

#endif