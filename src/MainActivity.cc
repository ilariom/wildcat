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
#include "core/systems/ScriptSystem.h"
#include "core/systems/MessageSystem.h"
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
    }

    void onMessage(const std::string& msg, const wkt::ecs::Entity& sender) override { s2x::log(msg); }
    void update(duration dt) override
    {
        auto entity = getEntity();
        auto transform = entity->query<wkt::components::Transform>();
        auto t = *transform;
        t->addPosition(wkt::math::vec2 { 5, 0 });
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

    scene->getDefaultSceneGraph().systemsManager().addSequential(std::make_unique<wkt::systems::ScriptSystem>());
    scene->getDefaultSceneGraph().systemsManager().addSequential(std::make_unique<wkt::systems::MessageSystem>());

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