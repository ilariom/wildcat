#ifndef _WKT_MOUSE_RECEIVER_H
#define _WKT_MOUSE_RECEIVER_H

#include "ecs/Component.h"
#include "input/MouseProxy.h"
#include <functional>

namespace wkt {
namespace components
{

class MouseReceiver : public wkt::ecs::Component
{
public:
    std::function<void(const wkt::events::MouseButtonEvent&)> onButton = nullptr;
    std::function<void(const wkt::events::MouseMotionEvent&)> onMotion = nullptr;
    std::function<void(const wkt::events::MouseWheelEvent&)> onWheel = nullptr; 
};

REGISTER_COMPONENT(MouseReceiver, -9);

}}

#endif