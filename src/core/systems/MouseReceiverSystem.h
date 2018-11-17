#ifndef _WKT_MOUSE_RECEIVER_SYSTEM_H
#define _WKT_MOUSE_RECEIVER_SYSTEM_H

#include "ecs/System.h"
#include "components/MouseReceiver.h"

namespace wkt {
namespace systems
{

class MouseReceiverSystem : public wkt::ecs::SequentialSystem<wkt::components::MouseReceiver>
{
public:
    MouseReceiverSystem();

public:
    void init() override;
    void operator()(std::shared_ptr<wkt::components::MouseReceiver>);
    void shutdown() override;

private:
    std::vector<wkt::events::MouseButtonEvent> buttonEvents;
    std::vector<wkt::events::MouseMotionEvent> motionEvents;
    std::vector<wkt::events::MouseWheelEvent> wheelEvents;
};

}}

#endif