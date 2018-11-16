#include "MainActivity.h"
#include "core/s2x/basics.h"
#include "core/globals/Director.h"
#include "core/globals/Scene.h"
#include "core/globals/SceneGraph.h"
#include "core/ecs/Entity.h"
#include "core/components/Node.h"
#include "core/components/Sprite.h"
#include "core/components/Transform.h"
#include "core/components/Script.h"
#include "core/components/KeyboardEventReceiver.h"
#include "core/systems/KeyboardReceiverSystem.h"
#include "core/systems/ScriptSystem.h"
#include "core/systems/MessageSystem.h"
#include "core/components/Dictionary.h"
#include "core/components/Table.h"
#include <memory>

class Mover : public wkt::components::Script
{
public:
    void init() override 
    { 
        s2x::log("INIT!");
        auto msg = makeMessage();
        msg.write("TEST MESSAGE");
        msg.sendTo(getEntity()->getUID());
        sendMessage(msg);
        scheduleUpdate();

        auto& entity = *getEntity();

        auto ker = std::make_shared<wkt::components::KeyboardEventReceiver>();
        ker->onKeyPressed = [&entity] (const wkt::events::KeyboardEventType& ket) {
            auto& dict = *std::static_pointer_cast<wkt::components::Dictionary<std::string, float>>(*entity.query<wkt::components::AbstractDictionary>());
            dict["x"] = 0;
            dict["y"] = 0;
            constexpr float step = 10;
            const Uint8* keyboardStateArray = SDL_GetKeyboardState(NULL);

            if(keyboardStateArray[SDL_SCANCODE_UP])
                dict["y"] -= step;
            else if(keyboardStateArray[SDL_SCANCODE_DOWN])
                dict["y"] += step;

            if(keyboardStateArray[SDL_SCANCODE_LEFT])
                dict["x"] -= step;
            else if(keyboardStateArray[SDL_SCANCODE_RIGHT])
                dict["x"] += step;
        };

        ker->onKeyDown = ker->onKeyPressed;
        ker->onKeyUp = ker->onKeyPressed;

        entity += ker;
    }

    void onMessage(const std::string& msg, const wkt::ecs::Entity& sender) override { s2x::log(msg); }
    void update(duration dt) override
    {
        auto entity = getEntity();
        auto keyboardEvent = entity->query<wkt::components::KeyboardReceiver>();
        auto transform = entity->query<wkt::components::Transform>();
        auto t = *transform;

        auto& dict = *std::static_pointer_cast<wkt::components::Dictionary<std::string, float>>(*entity->query<wkt::components::AbstractDictionary>());
        float x = dict["x"];
        float y = dict["y"];

        t->addPosition({ x, y });

        dict["x"] = 0;
        dict["y"] = 0;
    }
};

namespace wkt {
namespace components
{

REGISTER_SCRIPT(Mover);

}}

namespace wkt
{

void MainActivity::onCreate(StartupConfig& conf)
{
    conf.appName = "Wildcat Test App";
    conf.orgName = "Wildcat Developer";
    conf.windowWidth = 640;
    conf.windowHeight = 480;
    conf.isFullscreen = false;
    conf.hardwareAccelerated = true;
    conf.fps = 60;
    s2x::log("CREATE!");
}

void MainActivity::onStart()
{
    auto scene = std::make_shared<wkt::scene::Scene>();
    auto& entity = scene->getDefaultSceneGraph().entityManager().make();
    auto node = std::make_shared<wkt::components::Node>();
    auto transform = std::make_shared<wkt::components::Transform>();
    transform->setPosition({ 400, 100 });
    transform->setScale(.5f);
    transform->setRotation(45);
    auto sprite = std::make_shared<wkt::components::Sprite>("preview.png");
    entity += node;
    entity += transform;
    entity += sprite;
    scene->getDefaultSceneGraph().setRoot(node);

    auto& two = scene->getDefaultSceneGraph().entityManager().make();
    auto twon = std::make_shared<wkt::components::Node>();
    node->appendChild(twon);
    auto twot = std::make_shared<wkt::components::Transform>();
    twot->setScale(.25f);
    twot->setRotation(-45);
    auto twos = std::make_shared<wkt::components::Sprite>("ninja.png");
    ((two += twon) += twot) += twos;

    auto mover = std::make_shared<Mover>();
    two += mover;
    wkt::components::Dictionary<std::string, float> dict;
    two += wkt::components::make_abstract_dictionary_from(dict);

    scene->getDefaultSceneGraph().systemsManager() += std::make_unique<wkt::systems::ScriptSystem>();
    scene->getDefaultSceneGraph().systemsManager() += std::make_unique<wkt::systems::MessageSystem>();
    scene->getDefaultSceneGraph().systemsManager() += std::make_unique<wkt::systems::KeyboardEventReceiverSystem>();

    wkt::Director::getInstance().runScene(scene);
    s2x::log("START!");
}

void MainActivity::onResume()
{

}

void MainActivity::onPause()
{

}

void MainActivity::onStop()
{
    s2x::log("STOP!");
}

void MainActivity::onDestroy()
{
    s2x::log("DESTROY!");
}

}