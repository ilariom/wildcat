#ifndef _SKR_ENTITY_H
#define _SKR_ENTITY_H

#include "Component.h"
#include "ComponentsVector.h"
#include "Drawable.h"
#include "utils/Multiset.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>

namespace wkt {
namespace ecs 
{

class Entity final
{
public:
    using EntityUniqueID = unsigned long;
    using iterator = wkt::utils::Multiset<Component::ComponentTypeID>::iterator;

public:
    Entity();
    Entity(const Entity&);
    Entity(Entity&&) = default;
    ~Entity() = default;

    Entity& operator=(const Entity&) = delete;
    Entity& operator=(Entity&&) = default;

public:
    EntityUniqueID getUID() const { return this->id; }
    
    template<typename C = Component>
    Entity& operator+=(std::shared_ptr<C> comp)
    {
        using namespace wkt::components;

        comp->setEntity(this);

        if(comp->unique())
        {
            this->components[get_type_id<C>()].clear();
        }

        this->components[get_type_id<C>()].push_back(std::static_pointer_cast<Component>(comp));
        this->multiset.insert(get_type_id<C>());

        if(is_drawable<C>())
        {
            this->drws.push_back(std::static_pointer_cast<Drawable>(std::static_pointer_cast<Component>(comp)));
        }

        return *this;
    }

    Entity& operator-=(const Component& comp);
    
    Entity& remove(Component::ComponentUniqueID uid);
    Entity& removeAll(Component::ComponentTypeID typeId);

    template<typename C = Component>
    ComponentsVector<C> query(Component::ComponentTypeID typeId) const
    {
        ComponentsVector<C> vec;

        if(this->components.find(typeId) != this->components.end())
        {
            const ComponentsVector<>& cv = this->components.at(typeId);
            vec = cv;
        }

        return vec;
    }

    template<typename C>
    ComponentsVector<C> query() const
    {
        return query<C>(wkt::components::get_type_id<C>());
    }

    std::vector<std::shared_ptr<Drawable>>& drawables() { return this->drws; }
    iterator begin() { return this->multiset.begin(); }
    iterator end() { return this->multiset.end(); }

private:
    void removeDrawable(const Component& drw);

private:
    EntityUniqueID id;
    using MapOfComps = std::unordered_map<Component::ComponentTypeID, ComponentsVector<>>;
    MapOfComps components;
    std::vector<std::shared_ptr<Drawable>> drws;
    wkt::utils::Multiset<Component::ComponentTypeID> multiset;
};

}}

namespace std
{
    template<>
    struct hash<wkt::ecs::Entity>
    {
        size_t operator()(const wkt::ecs::Entity& e)
        {
            return std::hash<int>()(e.getUID());
        }
    };
}

#endif