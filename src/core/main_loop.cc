#include "main_loop.h"
#include "config.h"
#include "MainActivity.h"
#include "s2x/basics.h"
#include "s2x/video.h"
#include "s2x/events.h"
#include "input/InputManager.h"
#include "globals/World.h"
#include "globals/Scene.h"
#include "graphics/TextureCache.h"
#include "graphics/Director.h"
#include "input/KeyboardProxy.h"
#include "input/MouseProxy.h"
#include "input/ActionBroadcaster.h"
#include <chrono>
#include <thread>
#include <math.h>
#include <cassert>

namespace wkt
{

constexpr int QUIT_LISTENER_TAG = -1;
constexpr int KEYBOARD_PROXY_TAG = -2; 
constexpr int MOUSE_PROXY_TAG = -3;

void mainLoop()
{
    StartupConfig conf;
    MainActivity mainActivity;
    mainActivity.onCreate(conf);

    {
        s2x::Context context(SDL_INIT_VIDEO);
        s2x::Window window(conf.appName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, conf.windowWidth, conf.windowHeight);
        s2x::Renderer renderer(window, 
            conf.hardwareAccelerated ? SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC : SDL_RENDERER_SOFTWARE);

        wkt::gph::Director director(renderer);

        s2x::EventManager em;
        wkt::gph::TextureCache& textureCache = wkt::gph::TextureCache::getInstance();
        textureCache.setRenderer(&renderer);
        wkt::World& world = wkt::World::getInstance();

        double ratio = 1. / conf.fps;
        ratio *= 1000000;
        int64_t fr = static_cast<int64_t>(std::floor(ratio));
        auto frameRate = std::chrono::nanoseconds(fr);
        std::chrono::high_resolution_clock::time_point begin;
        std::chrono::high_resolution_clock::duration timeLapse;

        bool quit = false;

        s2x::EventListener el = {
            QUIT_LISTENER_TAG,
            [&quit] (const SDL_Event& ev) mutable {
                quit = true;
            }
        };

        em.addListener(SDL_QUIT, el);

        wkt::events::KeyboardProxy kl;
        em.addListener({ SDL_KEYDOWN, SDL_KEYUP }, {
            KEYBOARD_PROXY_TAG,
            [&kl] (const SDL_Event& ev) mutable {
                kl(ev);
            }
        });

        wkt::events::MouseProxy mp;
        em.addListener({ SDL_MOUSEBUTTONDOWN, SDL_MOUSEBUTTONUP, SDL_MOUSEMOTION, SDL_MOUSEWHEEL }, {
            MOUSE_PROXY_TAG,
            [&mp] (const SDL_Event& ev) mutable {
                mp(ev);
            }
        });

        wkt::events::InputManager& im = wkt::events::InputManager::getInstance();
        im.setKeyboardProxy(&kl);
        im.setMouseProxy(&mp);

        wkt::events::ActionBroadcaster& actionBroadcaster = im.getActionBroadcaster();

        mainActivity.onStart();

        while(!quit)
        {
            begin = std::chrono::high_resolution_clock::now();

            em.userInput();

            // systems

            world.runComponentSystems();
            auto& runningScene = wkt::scene::getRunningScene();
            runningScene.runComponentSystems();

            for(auto& sceneGraph : runningScene)
                sceneGraph.runAllSystems();

            // render systems

            renderer.clear();

            for(auto& sceneGraph : runningScene)
            {
                sceneGraph.setDirector(&director);
                sceneGraph.render();
            }

            renderer.present();

            // clean destroyed entities
            
            world.entityManager().clean();
            runningScene.entityManager().clean();

            for(auto& sceneGraph : runningScene)
                sceneGraph.entityManager().clean();

            // clear input buffers

            kl.close();
            mp.close();
            actionBroadcaster.close();

            timeLapse = std::chrono::high_resolution_clock::now() - begin;

            if(timeLapse < frameRate)
            {
                auto waitFor = timeLapse - frameRate;
                std::this_thread::sleep_for(waitFor);
            }
        }

        mainActivity.onStop();
    }

    mainActivity.onDestroy();
}

}