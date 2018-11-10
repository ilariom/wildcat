#ifndef _WKT_ENTITY_MANAGER_H
#define _WKT_ENTITY_MANAGER_H

#include "ecs/Entity.h"
#include <unordered_map>
#include <vector>

namespace wkt {
namespace managers
{

class EntityManager final
{
public:
    using iterator = std::unordered_map<wkt::ecs::Entity::EntityUniqueID, wkt::ecs::Entity>::iterator;

private:
    using Entity = wkt::ecs::Entity;

public:
    EntityManager() = default;
    EntityManager(const EntityManager&) = delete;
    EntityManager(EntityManager&&) = default;
    ~EntityManager() = default;

    EntityManager& operator=(const EntityManager&) = delete;
    EntityManager& operator=(EntityManager&&) = default;

public:
    Entity& make();
    void kill(Entity::EntityUniqueID id);
    void kill(const Entity& en);
    void gain(Entity&& en);

    Entity* operator[](Entity::EntityUniqueID id);

    void clean();

    iterator begin() { return this->entities.begin(); }
    iterator end() { return this->entities.end(); }

private:
    std::vector<Entity::EntityUniqueID> toBeKilled;
    std::unordered_map<Entity::EntityUniqueID, Entity> entities;
};


}}

#endif