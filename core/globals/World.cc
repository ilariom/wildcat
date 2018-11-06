#include "World.h"

namespace wkt
{

World& World::getInstance()
{
    static World world;
    return world;
}

}