#ifndef WKT_BOUNDING_BOX_UPDATE_SYSTEM_H
#define WKT_BOUNDING_BOX_UPDATE_SYSTEM_H

#include "ecs/System.h"
#include "components/BoundingBox.h"

namespace wkt {
namespace systems
{

class BoundingBoxUpdateSystem : public wkt::ecs::SequentialSystem<wkt::components::BoundingBox>
{
public:
    BoundingBoxUpdateSystem();

public:
    void operator()(std::shared_ptr<wkt::components::BoundingBox>);
};

}}

#endif