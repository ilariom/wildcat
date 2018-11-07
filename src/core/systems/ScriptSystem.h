#ifndef _WKT_SCRIPT_SYSTEM_H
#define _WKT_SCRIPT_SYSTEM_H

#include "ecs/System.h"

namespace wkt {
namespace systems
{

class ScriptSystem : public wkt::ecs::SystemDelegate
{
public:
    static wkt::ecs::System makeSystem();

public:
    void init() override { }
    void step(std::shared_ptr<wkt::ecs::Component>) override;
    bool step(wkt::components::Node&) override { }
    void shutdown() override { }
};

}}

#endif