#include "Entity.h"
#include <algorithm>

namespace wkt {
namespace ecs 
{

namespace {
    Entity::EntityUniqueID uid = 0;
}

Entity::Entity() : id(uid++)
{ }

Entity::Entity(const Entity& en)
{
    this->id = uid++;

    for(auto& p : en.components)
    {
        Component::ComponentTypeID id = p.first;
        auto& vec = p.second;
        std::for_each(vec.cbegin(), vec.cend(), [this, id] (const std::shared_ptr<wkt::ecs::Component>& cptr) {
            this->components[id].emplace_back(std::make_shared<wkt::ecs::Component>(*cptr.get()));
            this->components[id].back()->setEntity(this);
        });
    }

    this->multiset = en.multiset;
}

Entity& Entity::operator-=(const Component& comp)
{
    return remove(comp.getUID());
}

Entity& Entity::remove(Component::ComponentUniqueID uid)
{
    for(auto& p : this->components)
    {
        ComponentsVector<>& vec = p.second;
        std::vector<ComponentsVector<>::iterator> its;

        auto it = std::find_if(vec.begin(), vec.end(), [&uid] (const std::shared_ptr<Component>& c) {
            return c->getUID() == uid;
        });

        if(it != vec.end())
        {
            (*it)->setEntity(nullptr);
            removeDrawable(**it);
            vec.erase(it);
            this->multiset.erase(static_cast<const Component::ComponentTypeID>(p.first));
        }
    }

    return *this;
}

Entity& Entity::removeAll(Component::ComponentTypeID typeId)
{
    if(this->components.find(typeId) != this->components.end())
    {
        auto& comps = this->components[typeId];

        std::for_each(comps.begin(), comps.end(), [this] (std::shared_ptr<Component>& c) {
            removeDrawable(*c);
        });

        this->components[typeId].clear();
    }

    return *this;
}

void Entity::removeDrawable(const Component& drw)
{
    auto drwit = std::find_if(this->drws.begin(), this->drws.end(), [&drw] (std::shared_ptr<Drawable>& d) {
        return d->getUID() == drw.getUID();
    });

    if(drwit == this->drws.end())
        return;

    this->drws.erase(drwit); 
}
}}