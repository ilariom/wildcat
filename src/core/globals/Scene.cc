#include "Scene.h"
#include <cassert>

namespace
{
    std::shared_ptr<::wkt::scene::Scene> runningScene = nullptr;
}

namespace wkt {
namespace scene
{

wkt::scene::Scene& getRunningScene()
{
    assert(runningScene != nullptr);
    return *runningScene;
}

void runScene(std::shared_ptr<wkt::scene::Scene> scene)
{
    runningScene = scene;
}

}}