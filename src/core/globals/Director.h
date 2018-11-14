#ifndef _WKT_DIRECTOR_H
#define _WKT_DIRECTOR_H

#include <memory>

namespace wkt 
{

namespace scene { class Scene; }

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

private:
    Director() = default;

private:
    std::shared_ptr<wkt::scene::Scene> scene;
};

}

#endif