#ifndef WKT_SCRIPT_INTERFACE_H
#define WKT_SCRIPT_INTERFACE_H

#include "components/Script.h"
#include <functional>

namespace wkt {
namespace components
{

class ScriptInterface : public wkt::components::Script
{
public:
    struct Responder
    {
        std::function<void()> init = nullptr;
        std::function<void(const std::string&, const wkt::ecs::Entity&)> onMessage = nullptr;
        std::function<void(duration)> update = nullptr;
    };

public:
    inline void init() override;
    inline void onMessage(const std::string& msg, const wkt::ecs::Entity& sender) override;
    inline void update(duration dt) override;

public:
    Responder responder;
};

inline void ScriptInterface::init()
{
    if (!this->responder.init)
        return;

    this->responder.init();
}

inline void ScriptInterface::onMessage(const std::string& msg, const wkt::ecs::Entity& sender)
{
    if (!this->responder.onMessage)
        return;

    this->responder.onMessage(msg, sender);
}

inline void ScriptInterface::update(duration dt)
{
    if (!this->responder.update)
        return;

    this->responder.update(dt);
}

REGISTER_COMPONENT(ScriptInterface, -16);

}}

#endif