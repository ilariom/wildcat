#include "KeyboardReceiverSystem.h"
#include "input/InputManager.h"

using namespace wkt::ecs;
using namespace wkt::components;
using namespace wkt::events;

namespace wkt {
namespace systems
{

KeyboardReceiverSystem::KeyboardReceiverSystem()
{
    bindHandler(std::bind(&KeyboardReceiverSystem::operator(), this, std::placeholders::_1));
}

void KeyboardSystemBase::init()
{
    KeyboardProxy* kl = InputManager::getInstance().getKeyboardProxy();

    if(!kl)
        return;

    KeyboardProxy eventsStream = *kl;
    KeyboardEventType ket;

    while(eventsStream >> ket)
        this->events.push_back(std::move(ket));
}

void KeyboardSystemBase::shutdown()
{
    this->events.clear();
}

void KeyboardReceiverSystem::operator()(std::shared_ptr<wkt::components::KeyboardReceiver> ke)
{
    ke->addKeys(getEvents());
}

KeyboardEventReceiverSystem::KeyboardEventReceiverSystem()
{
    bindHandler(std::bind(&KeyboardEventReceiverSystem::operator(), this, std::placeholders::_1));
}

void KeyboardEventReceiverSystem::operator()(std::shared_ptr<wkt::components::KeyboardEventReceiver> ker)
{
    for(auto& event : getEvents())
        if(event.repeat && ker->onKeyPressed)
            ker->onKeyPressed(event);
        else if(event.eventType == SDL_KEYUP && ker->onKeyUp)
            ker->onKeyUp(event);
        else if(event.eventType == SDL_KEYDOWN && ker->onKeyDown)
            ker->onKeyDown(event);
}

}}