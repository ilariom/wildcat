#include "MainActivity.h"
#include "core/s2x/basics.h"
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
#include "core/pixmanip/pixmanip.h"
#include "core/components/JSON.h"
#include "core/components/Crowd.h"
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

        // auto transform = *entity.query<Transform>();
        // transform->setScale(.25f);
        // auto sprite = *entity.query<Sprite>();
        // sprite->shade(wkt::shaders::blackAndWhite(.3f, .3f, .4f));

        auto mouseRecv = std::make_shared<MouseReceiver>();
        mouseRecv->onButton = [this] (const wkt::events::MouseButtonEvent& ev) {
            s2x::log("CLICK");
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
    auto mt = std::make_shared<Transform>();
    mt->setScale(.5f);
    entity += node;
    entity += mt;
    entity += std::make_shared<Mover>();
    entity += std::make_shared<JSON>();
    entity += std::make_shared<Sprite>("urban.jpg");
    scene->getDefaultSceneGraph().setRoot(node);
    auto t1 = std::make_shared<Transform>();
    auto t2 = std::make_shared<Transform>();
    t1->setPosition({200, 100});
    t1->setScale(.5f);
    t2->setScale(.15f);

    auto crowd = std::make_shared<Crowd>();
    crowd->emplace("ninja.png", *t1);
    crowd->emplace("ninja.png", *t2);

    auto& e2 = scene->getDefaultSceneGraph().entityManager().make();
    auto e2node = std::make_shared<Node>();
    auto e2tr = std::make_shared<Transform>();
    node->appendChild(e2node);
    e2tr->setPosition({640, 480});
    e2 += crowd;
    e2 += e2node;
    e2 += e2tr;

    scene->getDefaultSceneGraph().systemsManager() += std::make_unique<wkt::systems::ScriptSystem>();
    scene->getDefaultSceneGraph().systemsManager() += std::make_unique<wkt::systems::MouseReceiverSystem>();

    wkt::scene::runScene(scene);
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
