#include <SDL2/SDL.h>
#include <iostream>
#include <thread>
#include <chrono>
#include "core/s2x/basics.h"
#include "core/s2x/video.h"
#include "core/s2x/events.h"
#include "core/managers/EntityManager.h"
#include "core/components/Node.h"
#include "core/components/Transform.h"
#include <memory>
#include "core/main_loop.h"

int main()
{
    wkt::mainLoop();

    // s2x::Context ctx(SDL_INIT_VIDEO);
    // s2x::Window window("TEST", 0, 0, 640, 480);
    // s2x::Renderer renderer(window, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    // s2x::Surface srf("../res/preview.png");
    // s2x::Texture tex(renderer, srf);

    // renderer.clear();
    // renderer.copy(tex, srf.size().width, srf.size().height);
    // renderer.present();

    // std::this_thread::sleep_for(std::chrono::seconds(3));

    // s2x::Context ctx(SDL_INIT_VIDEO);
    // s2x::Window window("TEST", 0, 0, 640, 480);
    // s2x::Renderer renderer(window, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    // s2x::Surface srf(32, 32);
    // s2x::Texture tex(renderer, srf);
    // bool quit = false;
    // uint8_t n = 0;

    // s2x::EventManager em;
    // s2x::EventListener el = {
    //     0,
    //     [&srf, &quit, &n, &tex] (const SDL_Event& e) mutable {
    //         if(e.key.keysym.sym == SDLK_UP)
    //         {
    //             n += 1;
    //             srf.fill({n, 0, 0, 255});
    //             tex = srf;
    //         }
    //         else if(e.key.keysym.sym == SDLK_DOWN)
    //         {
    //             n -= 1;
    //             srf.fill({n, 0, 0, 255});
    //             tex = srf;
    //         }
    //         else if(e.key.keysym.sym == SDLK_SPACE)
    //         {
    //             quit = true;
    //         }
    //     }
    // };

    // em.addListener(SDL_KEYDOWN, el);

    // while(!quit)
    // {
    //     em.userInput();
    //     renderer.clear();
    //     renderer.copy(tex, 3 * srf.size().width, 3 * srf.size().height);
    //     renderer.present();
    // }

}