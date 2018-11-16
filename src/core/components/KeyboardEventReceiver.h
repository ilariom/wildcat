#ifndef _WKT_KEYBOARD_EVENT_RECEIVER_H
#define _WKT_KEYBOARD_EVENT_RECEIVER_H

#include "ecs/Component.h"
#include "input/KeyboardProxy.h"

namespace wkt {
namespace components
{

class KeyboardEventReceiver : public wkt::ecs::Component
{
public:
    using callback_type = std::function<void(const wkt::events::KeyboardEventType& ket)>;

public:
    callback_type onKeyDown = nullptr;
    callback_type onKeyPressed = nullptr;
    callback_type onKeyUp = nullptr;
};

REGISTER_COMPONENT(KeyboardEventReceiver, -8);

}}

#endif