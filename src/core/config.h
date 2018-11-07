#ifndef _WKT_CONFIG_H
#define _WKT_CONFIG_H

#include <string>

namespace wkt
{

struct StartupConfig
{
    std::string appName;
    std::string orgName;
    int windowWidth;
    int windowHeight;
    bool isFullscreen;
    bool hardwareAccelerated;
    unsigned int fps;
};

}

#endif