#ifndef _WKT_SCENE_GRAPH_H
#define _WKT_SCENE_GRAPH_H

#include "managers/ManagersCollector.h"
#include "components/Node.h"
#include "globals/Director.h"
#include "globals/Camera.h"
#include "core/graphics/SurfaceCache.h"
#include "systems/RenderSystem.h"
#include "systems/TransformUpdateSystem.h"
#include <memory>

namespace wkt {
namespace scene
{

class SceneGraph final : public wkt::managers::ManagersCollector
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

    inline void runAllSystems();
    inline void render();

    void setActive(bool enable) { this->active = enable; }
    bool isActive() const { return this->active; }

private:
    std::shared_ptr<wkt::components::Node> node;
    std::unique_ptr<wkt::systems::RenderSystem> renderSystem;
    Camera camera;
    bool active = true;
};

inline SceneGraph::SceneGraph()
{ 
    this->camera.setSize({640.f, 480.f});
    this->camera.setPosition({200.f, 0.f});
    this->renderSystem = std::make_unique<wkt::systems::RenderSystem>(this->camera);
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

    this->renderSystem->bindRoot(*root());
    this->renderSystem->run();
}

}}

#endif