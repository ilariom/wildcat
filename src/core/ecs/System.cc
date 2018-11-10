#include "System.h"
#include <algorithm>

namespace wkt {
namespace ecs
{

void HierarchicalSystem::run()
{
    auto& root = getRoot();
    auto& handler = getHandler();

    init();
    root.visit(handler);
    shutdown();
}

}}