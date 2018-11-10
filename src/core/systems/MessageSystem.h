#ifndef _WKT_MESSAGE_SYSTEM_H
#define _WKT_MESSAGE_SYSTEM_H

#include "ecs/System.h"
#include "ecs/Entity.h"
#include "components/Script.h"
#include "utils/Message.h"
#include <vector>
#include <string>

namespace wkt {
namespace systems
{

class MessageSystem : public wkt::ecs::SequentialSystem<wkt::components::Script>
{
public:
    MessageSystem();

public:
    void operator()(std::shared_ptr<wkt::components::Script>);
    void shutdown() override;

private:
    std::vector<wkt::utils::Message<std::string>> messages;
    std::vector<wkt::ecs::Entity*> entities;
};

}}

#endif