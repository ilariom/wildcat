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
#include "core/components/MouseReceiver.h"
#include "core/systems/MouseReceiverSystem.h"
#include "core/components/ActionReceiver.h"
#include "core/systems/ActionReceiverSystem.h"
#include "core/input/InputManager.h"
#include "core/shaders/shaders.h"
#include "core/components/JSON.h"
#include <memory>

using namespace wkt::components;
using namespace wkt::events;
using namespace wkt::gph;
using namespace wkt::math;

class Mover : public Script
{
public:
    void init() override 
    { 
        s2x::log("INIT!");
        auto& entity = *getEntity();
        auto jsptr = *entity.query<JSON>();
        JSON js = *jsptr;
        js["hello"] = "world";
        s2x::log(js["hello"].asString());

        auto transform = *entity.query<Transform>();
        transform->setScale(.5f);
        auto sprite = *entity.query<Sprite>();
        sprite->shade(wkt::shaders::lighten(.5f));

        auto mouseRecv = std::make_shared<MouseReceiver>();
        mouseRecv->onButton = [this, sprite, transform] (const wkt::events::MouseButtonEvent& ev) {
            s2x::log("CLICK");
            sprite->resetShading();
            transform->setPosition({(float)ev.x, (float)ev.y});
        };

        entity += mouseRecv;
    }

    void onMessage(const std::string& msg, const wkt::ecs::Entity& sender) override { s2x::log(msg); }
    void update(duration dt) override
    {
       
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
    auto node = std::make_shared<Node>();
    entity += node;
    entity += std::make_shared<Transform>();
    entity += std::make_shared<Sprite>("ninja.png");
    entity += std::make_shared<Mover>();
    entity += std::make_shared<JSON>();
    scene->getDefaultSceneGraph().setRoot(node);

    auto& s = scene->getDefaultSceneGraph().entityManager().make();
    auto n = std::make_shared<Node>();
    node->appendChild(n);
    auto t = std::make_shared<Transform>();
    t->setPosition({300, 150});
    s += n;
    s += t;
    s += std::make_shared<Sprite>("ninja.png");
    
    scene->getDefaultSceneGraph().systemsManager() += std::make_unique<wkt::systems::ScriptSystem>();
    scene->getDefaultSceneGraph().systemsManager() += std::make_unique<wkt::systems::MouseReceiverSystem>();

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