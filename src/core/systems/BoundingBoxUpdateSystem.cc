#include "BoundingBoxUpdateSystem.h"

using namespace wkt::ecs;
using namespace wkt::components;

namespace wkt {
namespace systems
{

BoundingBoxUpdateSystem::BoundingBoxUpdateSystem()
{
    bindHandler(std::bind(&BoundingBoxUpdateSystem::operator(), this, std::placeholders::_1));
}

void BoundingBoxUpdateSystem::operator()(std::shared_ptr<wkt::components::BoundingBox> bb)
{
    bb->update();
}

}}