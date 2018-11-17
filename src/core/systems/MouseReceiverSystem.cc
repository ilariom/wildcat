#include "MouseReceiverSystem.h"
#include "input/InputManager.h"

using namespace wkt::ecs;
using namespace wkt::components;
using namespace wkt::events;

namespace wkt {
namespace systems
{

MouseReceiverSystem::MouseReceiverSystem()
{
    bindHandler(std::bind(&MouseReceiverSystem::operator(), this, std::placeholders::_1));
}

void MouseReceiverSystem::init()
{
    MouseProxy* mp = InputManager::getInstance().getMouseProxy();

    if(!mp)
        return;

    MouseProxy eventsStream = *mp;
    this->buttonEvents = eventsStream.takeButtonEvents();
    this->motionEvents = eventsStream.takeMotionEvents();
    this->wheelEvents = eventsStream.takeWheelEvents();
}

void MouseReceiverSystem::shutdown()
{
    this->buttonEvents.clear();
    this->motionEvents.clear();
    this->wheelEvents.clear();
}

void MouseReceiverSystem::operator()(std::shared_ptr<wkt::components::MouseReceiver> mr)
{
    if(mr->onButton)
        for(auto& ev : this->buttonEvents)
            mr->onButton(ev);

    if(mr->onMotion)
        for(auto& ev : this->motionEvents)
            mr->onMotion(ev);

    if(mr->onWheel)
        for(auto& ev : this->wheelEvents)
            mr->onWheel(ev);
}

}}