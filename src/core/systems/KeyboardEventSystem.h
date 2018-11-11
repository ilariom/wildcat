#ifndef _WKT_KEYBOARD_EVENT_SYSTEM_H
#define _WKT_KEYBOARD_EVENT_SYSTEM_H

#include "ecs/System.h"
#include "components/KeyboardEvent.h"
#include "input/KeyboardListener.h"
#include <vector>

namespace wkt {
namespace systems
{

class KeyboardEventSystem : public wkt::ecs::SequentialSystem<wkt::components::KeyboardEvent>
{
public:
    KeyboardEventSystem();

public:
    virtual void init() override;
    void operator()(std::shared_ptr<wkt::components::KeyboardEvent>);
    virtual void shutdown() override;

private:
    std::vector<wkt::events::KeyboardEventType> events;
};

}}

#endif