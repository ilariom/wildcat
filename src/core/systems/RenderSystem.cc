#include "RenderSystem.h"
#include "ecs/Drawable.h"
#include "graphics/TextureCache.h"
#include "components/Transform.h"
#include <memory>

using namespace wkt::ecs;
using namespace wkt::components;

namespace wkt {
namespace systems
{

RenderSystem::RenderSystem()
{
    bindHandler(std::bind(&RenderSystem::operator(), this, std::placeholders::_1));
}

bool RenderSystem::operator()(wkt::components::Node& node)
{
    Entity* entity = node.getEntity();

    if(!entity)
        return true;

    auto transforms = entity->query<Transform>();
    
    if(!transforms)
        return false;

    auto& drawables = entity->drawables();

    if(drawables.empty())
        return true;

    auto transform = *transforms;
    const wkt::gph::Director* dir = this->director;

    std::for_each(drawables.begin(), drawables.end(), [&transform, dir] (std::shared_ptr<Drawable>& d) {
        if(!d->isVisible())
            return;

        d->draw(*dir, *transform);
    });
    
    return true;
}

}}