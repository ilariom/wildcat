#ifndef _WKT_SYSTEMS_MANAGER_H
#define _WKT_SYSTEMS_MANAGER_H

#include "ecs/System.h"
#include "ecs/Entity.h"
#include "components/Node.h"
#include "managers/EntityManager.h"
#include <memory>
#include <vector>
#include <unordered_map>

namespace wkt {
namespace managers
{

class SystemsManager final
{
public:
    SystemsManager() = default;
    SystemsManager(const SystemsManager&) = delete;
    SystemsManager(SystemsManager&&) = default;
    ~SystemsManager() = default;

    SystemsManager& operator=(const SystemsManager&) = delete;
    SystemsManager& operator=(SystemsManager&&) = default;

public:
    SystemsManager& operator+=(std::unique_ptr<wkt::ecs::System> sys);

    void run(wkt::components::Node& node);
    void run(typename EntityManager::iterator begin, typename EntityManager::iterator);

private:
    std::vector<std::unique_ptr<wkt::ecs::System>> systems;
};

}}

#endif