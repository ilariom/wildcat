#include "ActionReceiverSystem.h"
#include "input/InputManager.h"
#include "input/ActionBroadcaster.h"

using namespace wkt::ecs;
using namespace wkt::components;
using namespace wkt::events;

namespace wkt {
namespace systems
{

ActionReceiverSystem::ActionReceiverSystem()
{
    bindHandler(std::bind(&ActionReceiverSystem::operator(), this, std::placeholders::_1));
}

void ActionReceiverSystem::init()
{
    ActionBroadcaster ab = InputManager::getInstance().getActionBroadcaster();
    this->actions = ab.takeActions();
}

void ActionReceiverSystem::operator()(std::shared_ptr<ActionReceiver> ar)
{
    if(!ar->onAction)
        return;

    for(auto& action : this->actions)
        if(ar->canReceive(action.name))
        ar->onAction(action);
}

void ActionReceiverSystem::shutdown()
{
   this->actions.clear();
}

}}