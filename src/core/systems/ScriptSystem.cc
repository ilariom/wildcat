#include "ScriptSystem.h"
#include "components/Script.h"
#include "ecs/Entity.h"
#include "ecs/Component.h"
#include <chrono>

using namespace wkt::ecs;
using namespace wkt::components;

namespace wkt {
namespace systems
{

void ScriptSystem::step(std::shared_ptr<Component> c)
{
    auto sc = comp_cast<Script>(c);
    auto now = std::chrono::high_resolution_clock::now();
    Script::time_point zeroTime(Script::duration(0));

    if(sc->getLastTimePoint() == zeroTime)
    {
        sc->init();
    }

    if(sc->isUpdateScheduled())
    {
        auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(now - sc->getLastTimePoint());
        sc->update(dt);
    }
    
    sc->setLastTimePoint(now);
}

System ScriptSystem::makeSystem()
{
    auto typeId = get_type_id<Script>();
    System system(typeId, std::make_unique<ScriptSystem>());
    return system;
}

}}