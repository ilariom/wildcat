#ifndef _WKT_SCRIPT_SYSTEM_H
#define _WKT_SCRIPT_SYSTEM_H

#include "ecs/System.h"
#include "components/Script.h"

namespace wkt {
namespace systems
{

class ScriptSystem : public wkt::ecs::SequentialSystem<wkt::components::Script>
{
public:
    ScriptSystem();

public:
    void operator()(std::shared_ptr<wkt::components::Script>);
};

}}

#endif