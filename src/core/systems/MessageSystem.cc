#include "MessageSystem.h"

using namespace wkt::ecs;
using namespace wkt::components;

namespace wkt {
namespace systems
{

MessageSystem::MessageSystem()
{
    bindHandler(std::bind(&MessageSystem::operator(), this, std::placeholders::_1));
}

void MessageSystem::operator()(std::shared_ptr<wkt::components::Script> sc)
{
    auto& msgs = sc->messages();
    this->messages.insert(this->messages.end(), msgs.begin(), msgs.end());
    msgs.clear();

    this->entities.push_back(sc->getEntity());
}

void MessageSystem::shutdown()
{
    using utils::Message;

    for(Entity* entity : this->entities)
        for(Message<std::string>& msg : this->messages)
            if(msg.isReceiver(*entity))
            {
                ComponentsVector<Script> scripts = entity->query<Script>();

                for(std::shared_ptr<Component> comp : scripts)
                {
                    auto sc = std::static_pointer_cast<Script>(comp);
                    sc->onMessage(msg.read(), *entity);
                }
            }

    this->messages.clear();
    this->entities.clear();
}

}}