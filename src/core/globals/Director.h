#ifndef _WKT_DIRECTOR_H
#define _WKT_DIRECTOR_H

#include <memory>

namespace wkt 
{

namespace scene { class Scene; }
namespace events { class KeyboardListener; }

class Director final
{
public:
    Director(const Director&) = delete;
    Director(Director&&) = delete;
    ~Director() = default;

    Director& operator=(const Director&) = delete;
    Director& operator=(Director&&) = delete;

public:
    static Director& getInstance();

public:
    wkt::scene::Scene& getRunningScene() { return *this->scene; }
    void runScene(std::shared_ptr<wkt::scene::Scene> scene) { this->scene = scene; }

    wkt::events::KeyboardListener* getKeyboardListener() const { return this->kl; }
    void setKeyboardListener(wkt::events::KeyboardListener& kl) { this->kl = &kl; }

private:
    Director() = default;

private:
    std::shared_ptr<wkt::scene::Scene> scene;
    wkt::events::KeyboardListener* kl = nullptr;
};

}

#endif