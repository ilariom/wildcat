#ifndef _SKR_SYSTEM_H
#define _SKR_SYSTEM_H

#include "Entity.h"
#include "Component.h"
#include "components/Node.h"
#include "managers/EntityManager.h"
#include <algorithm>
#include <unordered_map>

namespace wkt {
namespace ecs
{

class SystemDelegate
{
public:
    virtual void init() = 0;
    virtual void step(std::shared_ptr<Component>) = 0;
    virtual bool step(wkt::components::Node&) { return false; }
    virtual void shutdown() = 0;

    virtual bool isHierarchical() { return false; }
};

class System
{
public:
    System() = default;
    System(wkt::ecs::Component::ComponentTypeID typeId, std::unique_ptr<SystemDelegate> delegate) 
        : typeId(typeId), delegate(std::move(delegate))
    { }

public:

    void applyRule(typename wkt::managers::EntityManager::iterator begin, typename wkt::managers::EntityManager::iterator end)
    {
        this->delegate->init();

        for(auto it = begin; it != end; ++it)
        {
            auto& p = *it;
            auto vec = p.second.query(this->typeId);

            std::for_each(vec.begin(), vec.end(), [this] (std::shared_ptr<Component>& comp) {
                this->delegate->step(comp);
            });
        }

       this->delegate->shutdown();
    }

    void applyRule(wkt::components::Node& root)
    {
        this->delegate->init();

        root.visit([this] (wkt::components::Node& node) {
            return this->delegate->step(node);
        });

        this->delegate->shutdown();
    }

    Component::ComponentTypeID getTypeId() const { return this->typeId; }
    bool isHierarchical() const { return this->delegate->isHierarchical(); }

private:
    std::unique_ptr<SystemDelegate> delegate;
    Component::ComponentTypeID typeId;
};

}}

#endif