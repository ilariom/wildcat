#ifndef WKT_JSON_H
#define WKT_JSON_H

#include "ecs/Component.h"
#include "utils/json/webjson.h"

namespace wkt {
namespace components
{

class JSON : public wkt::ecs::Component, public webjson::Object
{ };

REGISTER_COMPONENT(JSON, -11);

}}

#endif