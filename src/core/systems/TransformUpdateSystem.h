#ifndef _WKT_TRANSFORM_UPDATE_SYSTEM_H
#define _WKT_TRANSFORM_UPDATE_SYSTEM_H

#include "ecs/System.h"
#include "components/Node.h"

namespace wkt {
namespace systems
{

class TransformUpdateSystem : public wkt::ecs::HierarchicalSystem
{
public:
    TransformUpdateSystem();

public:
    bool operator()(wkt::components::Node&);
};

}}

#endif