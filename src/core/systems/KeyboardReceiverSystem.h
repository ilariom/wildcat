#ifndef _WKT_KEYBOARD_EVENT_SYSTEM_H
#define _WKT_KEYBOARD_EVENT_SYSTEM_H

#include "ecs/System.h"
#include "components/KeyboardReceiver.h"
#include "input/KeyboardProxy.h"
#include <vector>

namespace wkt {
namespace systems
{

class KeyboardReceiverSystem : public wkt::ecs::SequentialSystem<wkt::components::KeyboardReceiver>
{
public:
    KeyboardReceiverSystem();

public:
    virtual void init() override;
    void operator()(std::shared_ptr<wkt::components::KeyboardReceiver>);
    virtual void shutdown() override;

private:
    std::vector<wkt::events::KeyboardEventType> events;
};

}}

#endif