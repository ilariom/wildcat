#ifndef _WKT_RENDER_SYSTEM_H
#define _WKT_RENDER_SYSTEM_H

#include "ecs/System.h"
#include "components/Node.h"
#include "graphics/Director.h"

namespace wkt {
namespace systems
{

class RenderSystem : public wkt::ecs::HierarchicalSystem
{
public:
    RenderSystem();

public:
    bool operator()(wkt::components::Node&);
    void setDirector(wkt::gph::Director* director) { this->director = director; }

private:
    const wkt::gph::Director* director;
};

}}

#endif