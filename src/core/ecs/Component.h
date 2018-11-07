#ifndef _SKR_COMPONENT_H
#define _SKR_COMPONENT_H

#include "comp_traits.h"
#include <functional>
#include <vector>
#include <memory>

namespace wkt {
namespace ecs 
{

class Entity;

class Component
{
public:
    using ComponentUniqueID = unsigned long;
    using ComponentTypeID = int32_t;

public:
    Component();
    Component(const Component&) = default;
    Component(Component&&) = default;
    virtual ~Component() = default;

public:
    ComponentUniqueID getUID() const { return this->id; }
    virtual bool unique() const;
    Entity* getEntity() const { return this->entity; }

private:
    friend class Entity;
    void setEntity(Entity* entity) { this->entity = entity; }

private:
    ComponentUniqueID id;
    Entity* entity;
};

inline bool operator==(const Component& a, const Component& b)
{
    return a.getUID() == b.getUID();
}

template<typename C, template<typename> class smart_ptr>
C* comp_cast(smart_ptr<Component> sp)
{
    return static_cast<C*>(sp.get());
}

}}

namespace std
{
    template<>
    struct hash<wkt::ecs::Component>
    {
        size_t operator()(const wkt::ecs::Component& c)
        {
            return std::hash<int>()(c.getUID());
        }
    };
}

#endif