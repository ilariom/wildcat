#include "main_loop.h"
#include "config.h"
#include "MainActivity.h"
#include "s2x/basics.h"
#include "s2x/video.h"
#include "s2x/events.h"
#include "input/InputManager.h"
#include "globals/Director.h"
#include "globals/World.h"
#include "globals/Scene.h"
#include "graphics/TextureCache.h"
#include "input/KeyboardProxy.h"
#include <chrono>
#include <thread>
#include <cmath>
#include <cassert>

namespace wkt
{

constexpr int QUIT_LISTENER_TAG = -1;
constexpr int KEYBOARD_PROXY_TAG = -2; 

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

        wkt::events::InputManager& im = wkt::events::InputManager::getInstance();
        im.setKeyboardProxy(&kl);
        
        wkt::Director& director = wkt::Director::getInstance();

        mainActivity.onStart();

        while(!quit)
        {
            begin = std::chrono::high_resolution_clock::now();

            em.userInput();

            // systems
            world.runComponentSystems();
            auto& runningScene = director.getRunningScene();
            runningScene.runComponentSystems();

            for(auto& sceneGraph : runningScene)
                sceneGraph.runAllSystems();

            // render systems

            renderer.clear();

            for(auto& sceneGraph : runningScene)
                sceneGraph.render();

            renderer.present();

            // clean destroyed entities
            
            world.entityManager().clean();
            runningScene.entityManager().clean();

            for(auto& sceneGraph : runningScene)
                sceneGraph.entityManager().clean();

            kl.close();

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