#include "Component.h"

namespace wkt {
namespace ecs 
{

namespace {
    Component::ComponentUniqueID uid = 0;
}

Component::Component() : id(uid++)
{ }

bool Component::unique() const
{
    return true;
}

}}