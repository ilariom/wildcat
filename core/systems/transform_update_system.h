#ifndef _WKT_TRANSFORM_UPDATE_SYSTEM_H
#define _WKT_TRANSFORM_UPDATE_SYSTEM_H

#include "ecs/System.h"

namespace wkt {
namespace systems
{

namespace transform_update_system
{
    wkt::ecs::System makeSystem();
}

}}

#endif