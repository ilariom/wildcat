#include "RenderSystem.h"
#include "ecs/Drawable.h"
#include "graphics/TextureCache.h"
#include "components/Transform.h"
#include "s2x/video.h"
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
    s2x::Renderer* renderer = wkt::gph::TextureCache::getInstance().renderer();

    std::for_each(drawables.begin(), drawables.end(), [&transform, renderer] (std::shared_ptr<Drawable>& d) {
        d->draw(*renderer, *transform);
    });
    
    return true;
}

}}