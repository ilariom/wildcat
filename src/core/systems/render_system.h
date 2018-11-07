#ifndef _WKT_RENDER_SYSTEM_H
#define _WKT_RENDER_SYSTEM_H

#include "core/ecs/System.h"
#include "core/components/Node.h"

namespace wkt {
namespace systems
{

namespace render_system
{

wkt::ecs::System makeSystem();

}

}}

#endif