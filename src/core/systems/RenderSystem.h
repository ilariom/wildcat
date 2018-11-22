#ifndef _WKT_RENDER_SYSTEM_H
#define _WKT_RENDER_SYSTEM_H

#include "core/ecs/System.h"
#include "core/components/Node.h"
#include "globals/Camera.h"

namespace wkt {
namespace systems
{

class RenderSystem : public wkt::ecs::HierarchicalSystem
{
public:
    RenderSystem(const wkt::scene::Camera& camera);

public:
    bool operator()(wkt::components::Node&);

private:
    const wkt::scene::Camera& camera;
};

}}

#endif