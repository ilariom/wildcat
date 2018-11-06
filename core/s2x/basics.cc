#include "basics.h"

namespace s2x 
{

namespace hints
{

void clearHints() { SDL_ClearHints(); }

bool setHint(const std::string& name, const std::string& value)
{
    return SDL_SetHint(name.c_str(), value.c_str());
}

bool setHint(const std::string& name, const std::string& value, SDL_HintPriority priority)
{
    return SDL_SetHintWithPriority(name.c_str(), value.c_str(), priority);
}

std::string getHint(const std::string& name)
{
    std::string value;
    const char *res = SDL_GetHint(name.c_str());

    if (res)
    {
        value = res;
    }

    return value;
}

}

namespace error
{

std::string get()
{
    return SDL_GetError();
}

void clear()
{
    SDL_ClearError();
}

void set(const std::string& msg)
{
    SDL_SetError(msg.c_str());
}

} // namespace error

std::string getBasePath()
{
    std::string bp;
    char* res = SDL_GetBasePath();

    if(res)
    {
        bp = res;
        SDL_free(res);
    }

    return bp;
}

std::string getPrefPath(const std::string& org, const std::string& app)
{
    std::string pp;
    char* res = SDL_GetPrefPath(org.c_str(), app.c_str());

    if(res)
    {
        pp = res;
        SDL_free(res);
    }

    return pp;
}

void log(const std::string& msg)
{
    SDL_Log(msg.c_str());
}

}