#ifndef _WKT_SCRIPT_SYSTEM_H
#define _WKT_SCRIPT_SYSTEM_H

#include "ecs/System.h"

namespace wkt {
namespace systems
{

namespace script_system
{
    wkt::ecs::System makeSystem();
}

}}

#endif