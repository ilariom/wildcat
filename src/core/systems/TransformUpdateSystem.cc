#include "TransformUpdateSystem.h"
#include "components/Node.h"
#include "components/Transform.h"
#include <memory>

using namespace wkt::ecs;
using namespace wkt::components;

namespace wkt {
namespace systems
{

TransformUpdateSystem::TransformUpdateSystem()
{
    bindHandler(std::bind(&TransformUpdateSystem::operator(), this, std::placeholders::_1));
}

bool TransformUpdateSystem::operator()(Node& node)
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

    const Node& parent = *node.getParent();
    std::shared_ptr<Transform> parentTransform = getTransform(parent);
    std::shared_ptr<Transform> nodeTransform = getTransform(node);

    nodeTransform->setParentCoordinates(parentTransform->getWorldCoordinates());

    return true;
}

}}