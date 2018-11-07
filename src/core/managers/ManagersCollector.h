#ifndef _WKT_MANAGERS_COLLECTOR_H
#define _WKT_MANAGERS_COLLECTOR_H

#include "EntityManager.h"
#include "SystemsManager.h"
#include "ecs/Entity.h"

namespace wkt {
namespace managers
{

class ManagersCollector
{
public:
    EntityManager& entityManager() { return this->em; }
    SystemsManager& systemsManager() { return this->sm; }

    inline void runComponentSystems();

private:
    EntityManager em;
    SystemsManager sm;
};

inline void ManagersCollector::runComponentSystems()
{
    this->sm.run(this->em.begin(), this->em.end());
}

}}

#endif