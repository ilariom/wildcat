#ifndef _SKR_SYSTEM_H
#define _SKR_SYSTEM_H

#include "Entity.h"
#include "Component.h"
#include "components/Node.h"
#include "managers/EntityManager.h"
#include "utils/recursors.h"
#include <unordered_map>
#include <memory>

namespace wkt {
namespace ecs
{

class System : public wkt::utils::AbstractRecursor
{
public:
    virtual ~System() = default;

public:
    virtual void init() { }
    virtual void shutdown() { }
};

class HierarchicalSystem : public System, public wkt::utils::HierarchicalRecursor<wkt::components::Node, bool>
{
public:
    void run() override;
};

class SequentialSystemBase : public System { };

template<typename C = Component>
class SequentialSystem : public SequentialSystemBase, 
                         public wkt::utils::SequentialRecursor<std::shared_ptr<C>, wkt::managers::EntityManager::iterator, void>
{
public:
    void run() override
    {
        using rec = wkt::utils::SequentialRecursor<std::shared_ptr<C>, wkt::managers::EntityManager::iterator, void>;

        typename rec::handler& hnd = this->getHandler();
        typename rec::iterator begin = this->begin();
        typename rec::iterator end = this->end();

        this->init();

        for(auto it = begin; it != end; ++it)
        {
            auto& p = *it;
            Entity& en = p.second;
            ComponentsVector<C> vec = en.query<C>();

            for(std::shared_ptr<Component> c : vec)
                hnd(std::static_pointer_cast<C>(c));
        }   

        this->shutdown();
    }
};

template<typename S, typename... Args>
std::unique_ptr<System> make_seq_system(Args... args)
{
    auto sys = std::make_unique<S>(std::forward(args)...);
    return std::unique_ptr<System>(static_cast<System*>(sys.release()));
}

}}

#endif