#include "transform_update_system.h"
#include "components/Node.h"
#include "components/Transform.h"

namespace wkt {
namespace systems
{

namespace transform_update_system
{

wkt::ecs::System makeSystem()
{
    using namespace wkt::ecs;
    using namespace wkt::components;

    wkt::ecs::System system(get_type_id<Node>());
    system.setNodeOperation([] (Node& node) -> bool {

        auto getTransform = [] (const Node& n) -> std::shared_ptr<Transform> {
            wkt::ecs::Entity* entity = n.getEntity();

            if(!entity)
                return nullptr;

            auto transforms = entity->query<Transform>();
            
            if(!transforms)
                return nullptr;

            auto transform = *transforms;
            return transform;
        };

        if(!node.hasParent())
            return true;

        auto& parent = *node.getParent();
        auto parentTransform = getTransform(parent);
        auto nodeTransform = getTransform(node);

        nodeTransform->setParentCoordinates(parentTransform->getWorldCoordinates());

        return true;
    });

    return system;
}

}

}}