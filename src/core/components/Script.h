#ifndef _WKT_SCRIPT_H
#define _WKT_SCRIPT_H

#include "ecs/Component.h"
#include "ecs/Entity.h"
#include "utils/Message.h"
#include <string>
#include <chrono>
#include <vector>

namespace wkt {
namespace components
{

class Script : public wkt::ecs::Component
{   
public:
    using duration = std::chrono::milliseconds;
    using time_point = std::chrono::high_resolution_clock::time_point;
    using Message = wkt::utils::Message<std::string>;
    using Poke = wkt::utils::Message<int>;

public:
    Script() = default;
    virtual ~Script() noexcept = default;

public:
    virtual void init() = 0;
    virtual void onMessage(const std::string& msg, const wkt::ecs::Entity& sender) = 0;
    virtual void onPoke(int poke, const wkt::ecs::Entity& sender) = 0;
    virtual void update(duration dt) = 0;

public:
    void scheduleUpdate() { this->updateScheduled = true; }
    void unscheduleUpdate() { this->updateScheduled = false; }
    bool isUpdateScheduled() const { return this->updateScheduled; }

public:
    const time_point& getLastTimePoint() const { return this->lastTimePoint; }
    void setLastTimePoint(time_point tp) { this->lastTimePoint = std::move(tp); }

protected:
    Message makeMessage() { return Message(getEntity()); }
    Poke makePoke() { return Poke(getEntity()); }

    void sendMessage(Message msg) { this->msgs.push_back(std::move(msg)); }
    void sendPoke(Poke p) { this->pokes.push_back(std::move(p)); }

private:
    time_point lastTimePoint = time_point(duration(0));
    bool updateScheduled = false;
    std::vector<Message> msgs;
    std::vector<Poke> pokes;
};

REGISTER_COMPONENT(Script, -4);

}}

#endif