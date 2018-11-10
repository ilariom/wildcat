#include "EntityManager.h"
#include <algorithm>

using Entity = wkt::ecs::Entity;

namespace wkt {
namespace managers
{

Entity& EntityManager::make()
{
    Entity en;
    auto id = en.getUID();
    this->entities[id] = std::move(en);

    return this->entities[id];
}

void EntityManager::kill(Entity::EntityUniqueID id)
{
    if(this->entities.find(id) == this->entities.end())
    {
        return;
    }

    this->toBeKilled.push_back(id);
}

void EntityManager::kill(const Entity& en)
{
    kill(en.getUID());
}

void EntityManager::gain(Entity&& en)
{
    this->entities[en.getUID()] = std::move(en);
}

Entity* EntityManager::operator[](Entity::EntityUniqueID id)
{
    if(this->entities.find(id) == this->entities.end())
    {
        return nullptr;
    }

    return &this->entities[id];
}

void EntityManager::clean()
{
    if(!this->toBeKilled.empty())
    {
        std::for_each(this->toBeKilled.begin(), this->toBeKilled.end(), [this] (Entity::EntityUniqueID id) {
            this->entities.erase(id);
        });

        this->toBeKilled.clear();
    }
}

}}