#include "script_system.h"
#include "components/Script.h"
#include "ecs/Entity.h"
#include "ecs/Component.h"
#include <chrono>

namespace wkt {
namespace systems
{

namespace script_system
{
    wkt::ecs::System makeSystem()
    {
        using namespace wkt::ecs;
        using namespace wkt::components;

        auto typeId = get_type_id<Script>();
        wkt::ecs::System system(typeId);
        
        system.setComponentOperation([] (std::shared_ptr<Component>& c) {
            
            // auto sc = static_cast<Script*>(c.get());
            auto sc = comp_cast<Script>(c);
            auto now = std::chrono::high_resolution_clock::now();
            Script::time_point zeroTime(Script::duration(0));

            if(sc->getLastTimePoint() == zeroTime)
            {
                sc->init();
                sc->setLastTimePoint(now);
            }

            if(sc->isUpdateScheduled())
            {
                auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(now - sc->getLastTimePoint());
                sc->update(dt);
            }
            
            sc->setLastTimePoint(now);
        });

        return system;
    }
}

}}