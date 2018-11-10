#include "ScriptSystem.h"
#include <chrono>

using namespace wkt::ecs;
using namespace wkt::components;

namespace wkt {
namespace systems
{

ScriptSystem::ScriptSystem()
{
    bindHandler(std::bind(&ScriptSystem::operator(), this, std::placeholders::_1));
}

void ScriptSystem::operator()(std::shared_ptr<wkt::components::Script> sc)
{
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

}}