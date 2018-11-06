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

class System
{
public:
    using CompOp = std::function<void(std::shared_ptr<Component>&)>;
    using NodeOp = std::function<bool(wkt::components::Node&)>;
    using UtilsOp = std::function<void(void)>;

public:
    System() = default;
    System(wkt::ecs::Component::ComponentTypeID typeId) : typeId(typeId) { }

public:

    void applyRule(typename wkt::managers::EntityManager::iterator begin, typename wkt::managers::EntityManager::iterator end)
    {
        if(!this->compOp)
            return;

        if(this->kickstarter)
            this->kickstarter();

        for(auto it = begin; it != end; ++it)
        {
            auto& p = *it;
            auto vec = p.second.query(this->typeId);

            std::for_each(vec.begin(), vec.end(), this->compOp);
        }

        if(this->shutdown)
            this->shutdown;
    }

    void applyRule(wkt::components::Node& root)
    {
        if(!this->nodeOp)
            return;

        if(this->kickstarter)
            this->kickstarter();

        root.visit(this->nodeOp);

        if(this->shutdown)
            this->shutdown;
    }

    void setComponentOperation(CompOp op) { this->compOp = op; }
    void setNodeOperation(NodeOp op) { this->nodeOp = op; }

    void setInitializer(UtilsOp op) { this->kickstarter = op; }
    void setFinalizer(UtilsOp op) { this->shutdown = op; }

    bool isComponentOperation() const { return this->compOp != nullptr; }

protected:
    Component::ComponentTypeID getTypeId() const { return this->typeId; }

private:
    Component::ComponentTypeID typeId;
    CompOp compOp = nullptr;
    NodeOp nodeOp = nullptr;
    UtilsOp kickstarter = nullptr;
    UtilsOp shutdown = nullptr;
};

}}

#endif