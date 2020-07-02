#include "main_loop.h"
#include "config.h"

namespace wkt {

wkt::StartupConfig& getStartupConfig()
{
    static wkt::StartupConfig conf;
    return conf;
}

} // namespace wkt

int main()
{
    wkt::mainLoop();
}