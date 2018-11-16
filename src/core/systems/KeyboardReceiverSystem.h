#ifndef _WKT_KEYBOARD_EVENT_SYSTEM_H
#define _WKT_KEYBOARD_EVENT_SYSTEM_H

#include "ecs/System.h"
#include "components/KeyboardReceiver.h"
#include "components/KeyboardEventReceiver.h"
#include "input/KeyboardProxy.h"
#include <vector>

namespace wkt {
namespace systems
{

class KeyboardSystemBase
{
public:
    void init();
    void shutdown();

public:
    std::vector<wkt::events::KeyboardEventType>& getEvents() { return this->events; }

private:
    std::vector<wkt::events::KeyboardEventType> events;
};

class KeyboardReceiverSystem : public wkt::ecs::SequentialSystem<wkt::components::KeyboardReceiver>
{
public:
    KeyboardReceiverSystem();

public:
    void init() override { this->ksb.init(); }
    void operator()(std::shared_ptr<wkt::components::KeyboardReceiver>);
    void shutdown() override { this->ksb.shutdown(); }

private:
    std::vector<wkt::events::KeyboardEventType>& getEvents() { return this->ksb.getEvents(); }

private:
    KeyboardSystemBase ksb;
};

class KeyboardEventReceiverSystem : public wkt::ecs::SequentialSystem<wkt::components::KeyboardEventReceiver>
{
public:
    KeyboardEventReceiverSystem();

public:
    void init() override { this->ksb.init(); }
    void operator()(std::shared_ptr<wkt::components::KeyboardEventReceiver>);
    void shutdown() override { this->ksb.shutdown(); }

private:
    std::vector<wkt::events::KeyboardEventType>& getEvents() { return this->ksb.getEvents(); }

private:
    KeyboardSystemBase ksb;
};

}}

#endif