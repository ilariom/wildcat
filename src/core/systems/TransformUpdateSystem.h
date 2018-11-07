#ifndef _WKT_TRANSFORM_UPDATE_SYSTEM_H
#define _WKT_TRANSFORM_UPDATE_SYSTEM_H

#include "ecs/System.h"
#include "components/Node.h"

namespace wkt {
namespace systems
{

class TransformUpdateSystem : public wkt::ecs::SystemDelegate
{
public:
    static wkt::ecs::System makeSystem();

public:
    void init() override { }
    void step(std::shared_ptr<wkt::ecs::Component>) override { }
    bool step(wkt::components::Node&) override;
    void shutdown() override { }

    bool isHierarchical() override { return true; }
};

}}

#endif