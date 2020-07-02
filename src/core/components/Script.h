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

public:
    Script() = default;
    virtual ~Script() noexcept = default;

public:
    virtual void init() = 0;
    virtual void onMessage(const std::string& msg, const wkt::ecs::Entity& sender) = 0;
    virtual void update(duration dt) = 0;

public:
    void scheduleUpdate() { this->updateScheduled = true; }
    void unscheduleUpdate() { this->updateScheduled = false; }
    bool isUpdateScheduled() const { return this->updateScheduled; }

    bool unique() const override { return false; }

public:
    const time_point& getLastTimePoint() const { return this->lastTimePoint; }
    void setLastTimePoint(time_point tp) { this->lastTimePoint = std::move(tp); }

    std::vector<Message>& messages() { return this->msgs; }

protected:
    Message makeMessage() { return Message(getEntity()); }
    void sendMessage(Message msg) { this->msgs.push_back(std::move(msg)); }

private:
    time_point lastTimePoint = time_point(duration(0));
    bool updateScheduled = false;
    std::vector<Message> msgs;
};

REGISTER_COMPONENT(Script, -4);

}}

#endif