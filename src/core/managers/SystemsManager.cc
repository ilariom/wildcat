#include "SystemsManager.h"
#include <algorithm>

namespace wkt {
namespace managers
{

void SystemsManager::run(wkt::components::Node& node)
{
    std::for_each(this->hSystems.begin(), this->hSystems.end(), [&node] (std::unique_ptr<wkt::ecs::HierarchicalSystem>& sys) mutable {
        sys->bindRoot(node);
        sys->run();
    });
}

void SystemsManager::run(typename EntityManager::iterator begin, typename EntityManager::iterator end)
{
    std::for_each(this->systems.begin(), this->systems.end(), [&begin, &end] (std::unique_ptr<wkt::ecs::System>& sys) mutable {
        auto seqSys = static_cast<wkt::ecs::SequentialSystem<>*>(sys.get());
        seqSys->bindIterators(begin, end);
        sys->run();
    });
}

}}