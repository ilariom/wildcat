#ifndef _WKT_SCENE_GRAPH_H
#define _WKT_SCENE_GRAPH_H

#include "managers/ECSContext.h"
#include "components/Node.h"
#include "graphics/SurfaceCache.h"
#include "graphics/Camera.h"
#include "graphics/Director.h"
#include "systems/RenderSystem.h"
#include "systems/TransformUpdateSystem.h"
#include "config.h"
#include <memory>

namespace wkt {
namespace scene
{

class SceneGraph final : public wkt::managers::ECSContext
{
public:
    inline SceneGraph();
    SceneGraph(const SceneGraph&) = delete;
    SceneGraph(SceneGraph&&) = default;
    ~SceneGraph() = default;

    SceneGraph& operator=(const SceneGraph&) = delete;
    SceneGraph& operator=(SceneGraph&&) = default;

public:
    void setRoot(std::shared_ptr<wkt::components::Node> node) { this->node = node; }
    std::shared_ptr<wkt::components::Node> root() const { return this->node; }
    void setDirector(wkt::gph::Director* dir) { this->dir = dir; }
    wkt::gph::Director* director() const { return this->dir; }

    wkt::gph::Camera& camera() { return this->cam; }

    inline void runAllSystems();
    inline void render();

    void setActive(bool enable) { this->active = enable; }
    bool isActive() const { return this->active; }

private:
    std::shared_ptr<wkt::components::Node> node;
    std::unique_ptr<wkt::systems::RenderSystem> renderSystem;
    wkt::gph::Camera cam;
    wkt::gph::Director* dir = nullptr;
    bool active = true;
};

inline SceneGraph::SceneGraph()
{ 
    // this->cam.setSize({640.f, 480.f});
    // this->cam.setPosition({300.f, 0.f});
    this->cam.setSize({
        static_cast<float>(getStartupConfig().windowWidth),
        static_cast<float>(getStartupConfig().windowHeight)
    });

    this->renderSystem = std::make_unique<wkt::systems::RenderSystem>();
    auto& sm = systemsManager();
    sm += std::make_unique<wkt::systems::TransformUpdateSystem>();
}

inline void SceneGraph::runAllSystems()
{
    if(!isActive())
        return;

    runComponentSystems();

    if(root())
    {
        systemsManager().run(*root());
    }
}

inline void SceneGraph::render()
{
    if(!root() || !isActive())
        return;

    this->dir->setCamera(&this->cam);
    this->renderSystem->setDirector(this->dir);
    this->renderSystem->bindRoot(*root());
    this->renderSystem->run();
}

}}

#endif