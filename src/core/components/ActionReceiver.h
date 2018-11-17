#ifndef WKT_ACTION_RECEIVER_H
#define WKT_ACTION_RECEIVER_H

#include "s2x/basics.h"
#include "ecs/Component.h"
#include <functional>
#include <string>
#include <vector>

namespace wkt {
namespace components
{

struct Action
{
    using Payload = s2x::UserData;

    std::string name;
    Payload payload;
};

class ActionReceiver : public wkt::ecs::Component
{
public:
    bool unique() const override { return false; }

    std::function<void(const Action&)> onAction = nullptr;
    std::vector<std::string> filters;

    inline bool canReceive(const std::string& actionName);
};

inline bool ActionReceiver::canReceive(const std::string& actionName)
{
    return this->filters.empty() 
        || std::find(this->filters.begin(), this->filters.end(), actionName) != this->filters.end();
}

REGISTER_COMPONENT(ActionReceiver, -10);

}}

#endif