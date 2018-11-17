#ifndef _WKT_ACTION_RECEIVER_SYSTEM_H
#define _WKT_ACTION_RECEIVER_SYSTEM_H

#include "ecs/System.h"
#include "components/ActionReceiver.h"
#include <vector>

namespace wkt {
namespace systems
{

class ActionReceiverSystem : public wkt::ecs::SequentialSystem<wkt::components::ActionReceiver>
{
public:
    ActionReceiverSystem();

public:
    void init() override;
    void operator()(std::shared_ptr<wkt::components::ActionReceiver>);
    void shutdown() override;

private:
    std::vector<wkt::components::Action> actions;
};

}}

#endif