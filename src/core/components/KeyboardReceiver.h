#ifndef _WKT_KEYBOARD_EVENT_H
#define _WKT_KEYBOARD_EVENT_H

#include "ecs/Component.h"
#include "input/KeyboardProxy.h"
#include <vector>

namespace wkt {
namespace components
{

class KeyboardReceiver : public wkt::ecs::Component
{
public:
    std::vector<wkt::events::KeyboardEventType>&& consume() { return std::move(this->keys); }
    void addKeys(const std::vector<wkt::events::KeyboardEventType>& outer) { this->keys.insert(this->keys.end(), outer.begin(), outer.end()); }

private:
    std::vector<wkt::events::KeyboardEventType> keys;
};

REGISTER_COMPONENT(KeyboardReceiver, -5);

}}

#endif