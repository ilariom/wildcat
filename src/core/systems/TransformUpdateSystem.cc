#include "TransformUpdateSystem.h"
#include "components/Node.h"
#include "components/Transform.h"
#include <memory>

using namespace wkt::ecs;
using namespace wkt::components;

namespace wkt {
namespace systems
{

bool TransformUpdateSystem::step(Node& node)
{
    auto getTransform = [] (const Node& n) -> std::shared_ptr<Transform> {
        Entity* entity = n.getEntity();

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
}

System TransformUpdateSystem::makeSystem()
{
    System system(get_type_id<Node>(), std::make_unique<TransformUpdateSystem>());
    return system;
}

}}