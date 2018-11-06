#include "render_system.h"
#include "core/components/Sprite.h"
#include "core/graphics/TextureCache.h"
#include "core/components/Transform.h"
#include "core/s2x/video.h"
namespace wkt {
namespace systems
{

wkt::ecs::System render_system::makeSystem()
{
    using namespace wkt::components;

    wkt::ecs::System system(get_type_id<Node>());

    system.setNodeOperation([] (wkt::components::Node& node) -> bool {
        wkt::ecs::Entity* entity = node.getEntity();

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

        std::for_each(drawables.begin(), drawables.end(), [&transform, renderer] (std::shared_ptr<wkt::ecs::Drawable>& d) {
            d->draw(*renderer, *transform);
        });
        
        return true;
    });

    return system;
}

}}