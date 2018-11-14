#include "KeyboardEventSystem.h"
#include "input/InputManager.h"

using namespace wkt::ecs;
using namespace wkt::components;
using namespace wkt::events;

namespace wkt {
namespace systems
{

KeyboardEventSystem::KeyboardEventSystem()
{
    bindHandler(std::bind(&KeyboardEventSystem::operator(), this, std::placeholders::_1));
}

void KeyboardEventSystem::init()
{
    KeyboardProxy* kl = InputManager::getInstance().getKeyboardProxy();

    if(!kl)
        return;

    KeyboardProxy& eventsStream = *kl;
    KeyboardEventType ket;

    while(eventsStream >> ket)
        this->events.push_back(std::move(ket));
}

void KeyboardEventSystem::operator()(std::shared_ptr<wkt::components::KeyboardReceiver> ke)
{
    ke->addKeys(this->events);
}

void KeyboardEventSystem::shutdown()
{
    this->events.clear();
}

}}