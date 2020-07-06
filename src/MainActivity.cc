#include "MainActivity.h"
#include <s2x/basics.h>
#include <globals/Scene.h>
#include <globals/SceneGraph.h>
#include <ecs/Entity.h>
#include <components/Node.h>
#include <components/Sprite.h>
#include <components/Transform.h>
#include <components/Script.h>
#include <components/KeyboardEventReceiver.h>
#include <systems/KeyboardReceiverSystem.h>
#include <systems/ScriptSystem.h>
#include <systems/MessageSystem.h>
#include <components/Dictionary.h>
#include <components/Table.h>
#include <components/MouseReceiver.h>
#include <systems/MouseReceiverSystem.h>
#include <components/ActionReceiver.h>
#include <systems/ActionReceiverSystem.h>
#include <input/InputManager.h>
#include <pixmanip/pixmanip.h>
#include <components/JSON.h>
#include <components/Crowd.h>
#include <components/Text.h>
#include <graphics/Flipbook.h>
#include <graphics/Atlas.h>
#include <components/ScriptInterface.h>
#include <utils/search_path.h>
#include <scripts/Animator.h>
#include <memory>
#include <iostream>

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

        auto sprite = *entity.query<Sprite>();
        // sprite->shade(wkt::pixmanip::blackAndWhite(.3f, .3f, .4f));

        auto mouseRecv = std::make_shared<MouseReceiver>();
        mouseRecv->onButton = [this, sprite] (const wkt::events::MouseButtonEvent& ev) {
            if(ev.event == wkt::events::ButtonEvent::UP)
                sprite->shade(wkt::pixmanip::darken(.9f));
        };

        Atlas atlas;
        Atlas::Card card;

        Atlas::atlas_iterator it = atlas.end();

        // auto anim = *getEntity()->query<scripts::FlipbookAnimator>();
        // anim->start();
        // wkt::math::Rect r;
        // r.origin.x = 0;
        // r.origin.y = 0;
        // r.size.width = 20;
        // r.size.height = 20;

        // wkt::math::Rect s;
        // s.origin.x = 30;
        // r.origin.y = 0;
        // r.size.width = 200;
        // r.size.height = 200;

        // this->fc.addChannel(Flipbook {
        //     {
        //         { r, 100 },
        //         { s, 30 }
        //     }
        // });

        // this->fc.setChannel(0);

        entity += mouseRecv;

        scheduleUpdate();
    }

    void onMessage(const std::string& msg, const wkt::ecs::Entity& sender) override { s2x::log(msg); }
    void update(duration dt) override
    {
        // if (fc.hasNext())
        // {
        //     auto ninja = *getEntity()->query<Sprite>();
        //     ninja->crop(fc.next().rect);
        // }
    }

private:
    FlipbookChannels fc;
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

    wkt::path::add("../res");
}

void MainActivity::onStart()
{
    auto scene = std::make_shared<wkt::scene::Scene>();
    auto& entity = scene->getDefaultSceneGraph().entityManager().make();
    auto node = std::make_shared<Node>();
    auto mt = std::make_shared<Transform>();
    entity += node;
    entity += mt;
    // entity += std::make_shared<JSON>();
    auto s = std::make_shared<Sprite>("ninja.png");
    entity += s;

    // mt->setScale(.5f);
    // mt->setRotation(45);
    // mt->setRotationAnchor({
    //     .5f, .5f
    // });

    mt->setPosition({
        0, 0
    });
    // float sc = 640.f / s->size().width;
    // mt->setScale(sc);
    scene->getDefaultSceneGraph().setRoot(node);

    auto& ninja = scene->getDefaultSceneGraph().entityManager().make();
    auto ninjat = std::make_shared<Transform>();
    auto ninjan = std::make_shared<Node>();
    auto ninjas = std::make_shared<Sprite>("ninja.png");
    node->appendChild(ninjan);
    // // ninjat->setScale(1.f / sc);
    // ninjat->setScale(.5f);
    // ninjat->setPosition({320.f / sc, 0});
    ninja += ninjan;
    ninja += ninjat;
    ninja += ninjas;

    ninjat->setScale(.5f);
    ninjat->setPosition({
        100,
        480 - ninjat->getScale() * ninjas->size().height
    });

    ninjat->setRotationAnchor({
        .5f, .5f
    });

    ninjat->setRotation(90);

    scene->getDefaultSceneGraph().camera().setRotation(-20);
    scene->getDefaultSceneGraph().camera().setPosition({ 50, 0 });

    // ninja += std::make_shared<Mover>();

    // wkt::math::Rect r;
    // r.origin.x = 0;
    // r.origin.y = 0;
    // r.size.width = 20;
    // r.size.height = 20;

    // wkt::math::Rect r2;
    // r2.origin.x = 30;
    // r2.origin.y = 0;
    // r2.size.width = 200;
    // r2.size.height = 200;

    // FlipbookChannels fc;

    // fc.addChannel(Flipbook {
    //     {
    //         { r, 100 },
    //         { r2, 30 }
    //     }
    // });

    // auto anim = std::make_shared<scripts::FlipbookAnimator>(
    //     *ninjas,
    //     std::move(fc)
    // );

    // // ninja += anim;

    // anim->getFlipbookChannels().setChannel(0, true);
    // // anim->start();

    // auto kan = std::make_shared<scripts::Animator>(
    //     *ninjat,
    //     true
    // );

    // Coords A;
    // A.position = { 500.f / sc, 0 };
    // A.scaleX = A.scaleY = .5f;

    // kan->getKeyframes().emplace_back(
    //     8,
    //     A
    // );

    // ninja += kan;
    // kan->start();

    // std::cout << ninja.query<Script>().size() << std::endl;
    // auto& txten = scene->getDefaultSceneGraph().entityManager().make();
    // auto txtent = std::make_shared<Transform>();
    // auto txtenn = std::make_shared<Node>();
    // node->appendChild(txtenn);
    

    // auto t1 = std::make_shared<Transform>();
    // auto t2 = std::make_shared<Transform>();
    // t1->setPosition({200, 100});
    // t1->setScale(1);
    // t2->setScale(.5f);

    // auto crowd = std::make_shared<Crowd>();
    // crowd->emplace("ninja.png", *t1);
    // crowd->emplace("ninja.png", *t2);

    // auto& e2 = scene->getDefaultSceneGraph().entityManager().make();
    // auto e2node = std::make_shared<Node>();
    // auto e2tr = std::make_shared<Transform>();
    // // node->appendChild(e2node);
    // e2tr->setPosition({640, 480});
    // e2 += crowd;
    // e2 += e2node;
    // e2 += e2tr;

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
