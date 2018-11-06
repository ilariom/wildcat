#include "SystemsManager.h"
#include <algorithm>

namespace wkt {
namespace managers
{

SystemsManager& SystemsManager::operator+=(std::unique_ptr<wkt::ecs::System> sys)
{
    this->systems.push_back(std::move(sys));
}

void SystemsManager::run(wkt::components::Node& node)
{
    std::for_each(this->systems.begin(), this->systems.end(), [&node] (std::unique_ptr<wkt::ecs::System>& sys) mutable {
        if(!sys->isComponentOperation())
            sys->applyRule(node);
    });
}

void SystemsManager::run(typename EntityManager::iterator begin, typename EntityManager::iterator end)
{
    std::for_each(this->systems.begin(), this->systems.end(), [&begin, &end] (std::unique_ptr<wkt::ecs::System>& sys) mutable {
        if(sys->isComponentOperation())
            sys->applyRule(begin, end);
    });
}

}}