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

void KeyboardReceiverSystem::init()
{
    KeyboardProxy* kl = InputManager::getInstance().getKeyboardProxy();

    if(!kl)
        return;

    KeyboardProxy eventsStream = *kl;
    KeyboardEventType ket;

    while(eventsStream >> ket)
        this->events.push_back(ket);
}

void KeyboardReceiverSystem::operator()(std::shared_ptr<wkt::components::KeyboardReceiver> ke)
{
    ke->addKeys(this->events);
}

void KeyboardReceiverSystem::shutdown()
{
    this->events.clear();
}

}}