#ifndef _WKT_SCENE_H
#define _WKT_SCENE_H

#include "SceneGraph.h"
#include "managers/ManagersCollector.h"
#include <vector>
#include <algorithm>

namespace wkt {
namespace scene
{

class Scene final : public wkt::managers::ManagersCollector
{
public:
    using SceneGraphId = std::vector<SceneGraph>::size_type;
    using iterator = std::vector<SceneGraph>::iterator;

public:
    Scene() { push(); };
    Scene(const Scene&) = delete;
    Scene(Scene&&) = delete;
    ~Scene() = default;

    Scene& operator=(const Scene&) = delete;
    Scene& operator=(Scene&&) = delete;

public:
    SceneGraph& operator[](SceneGraphId id) { return this->sceneGraphs[id]; }
    inline SceneGraphId push();
    inline SceneGraphId pop();
    inline void erase(SceneGraphId id);

    inline SceneGraph& getDefaultSceneGraph() { return this->sceneGraphs[0]; }

    iterator begin() { return this->sceneGraphs.begin(); }
    iterator end() { return this->sceneGraphs.end(); }

private:
    std::vector<SceneGraph> sceneGraphs;
};

inline Scene::SceneGraphId Scene::push()
{
    this->sceneGraphs.emplace_back();
    return this->sceneGraphs.size() - 1;
}

inline Scene::SceneGraphId Scene::pop()
{
    if(this->sceneGraphs.size() > 1)
        this->sceneGraphs.pop_back();
        
    return this->sceneGraphs.size();
}

inline void Scene::erase(Scene::SceneGraphId id)
{
    this->sceneGraphs.erase(this->sceneGraphs.begin() + id);
}

wkt::scene::Scene& getRunningScene();
void runScene(std::shared_ptr<wkt::scene::Scene> scene);

}}

#endif