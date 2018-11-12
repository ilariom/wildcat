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
#include "core/components/KeyboardEvent.h"
#include "core/systems/KeyboardEventSystem.h"
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
    }

    void onMessage(const std::string& msg, const wkt::ecs::Entity& sender) override { s2x::log(msg); }
    void update(duration dt) override
    {
        auto entity = getEntity();
        auto keyboardEvent = entity->query<wkt::components::KeyboardEvent>();
        auto transform = entity->query<wkt::components::Transform>();
        auto t = *transform;

        auto v = (*keyboardEvent)->consume();

        float x = 0, y = 0;

        for(auto& ev : v)
            switch(ev.code)
            {
                case SDLK_LEFT:
                    x -= 10;
                    break;

                case SDLK_RIGHT:
                    x += 10;
                    break;

                case SDLK_UP:
                    y -= 10;
                    break;

                case SDLK_DOWN:
                    y += 10;
                    break;
            }

        t->addPosition({ x, y });
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

    two += std::make_shared<wkt::components::KeyboardEvent>();

    wkt::components::Dictionary<std::string, int> dict;
    dict["one"] = 1;
    dict["two"] = 2;

    two += wkt::components::make_abstract_dictionary_from(dict);

    using MyScheme = wkt::components::Scheme<std::string, int, double>;
    MyScheme sch("my_scheme");
    sch.update("Arnold", 1, 2.3);
    sch.update("Arnold", 2, 2.3);
    sch.update("Sly", 2, 2.3);
    auto&& res = sch.selectWhere<0>("Arnold");

    two += wkt::components::make_table_from_scheme(sch);

    scene->getDefaultSceneGraph().systemsManager().addSequential(std::make_unique<wkt::systems::ScriptSystem>());
    scene->getDefaultSceneGraph().systemsManager().addSequential(std::make_unique<wkt::systems::MessageSystem>());
    scene->getDefaultSceneGraph().systemsManager().addSequential(std::make_unique<wkt::systems::KeyboardEventSystem>());

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