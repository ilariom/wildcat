#ifndef _WKT_SCENE_GRAPH_H
#define _WKT_SCENE_GRAPH_H

#include "managers/ManagersCollector.h"
#include "components/Node.h"
#include "globals/Director.h"
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
    wkt::ecs::System system;
    bool active = true;
};

inline SceneGraph::SceneGraph()
{ 
    this->system = wkt::systems::RenderSystem::makeSystem();
    auto& sm = systemsManager();
    sm += std::make_unique<wkt::ecs::System>(wkt::systems::TransformUpdateSystem::makeSystem());
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

    this->system.applyRule(*root());
}

}}

#endif