#include "Component.h"

namespace wkt {
namespace ecs 
{

namespace {
    Component::ComponentUniqueID uid = 0;
}

Component::Component() : id(uid++)
{ }

Component::Component(const Component& comp)
{
    this->id = uid++;
    this->entity = comp.entity;
}

Component& Component::operator=(const Component& comp)
{
    this->id = uid++;
    this->entity = comp.entity;
}

bool Component::unique() const
{
    return true;
}

}}