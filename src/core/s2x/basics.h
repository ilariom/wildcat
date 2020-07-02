#ifndef _S2X_BASICS_H
#define _S2X_BASICS_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

namespace s2x
{

class Context
{
public:
    explicit inline Context(int flags);
    inline ~Context();
    Context(const Context &) = delete;
    Context(Context &&) = delete;

    Context &operator=(const Context &) = delete;
    Context &operator=(Context &&) = delete;
};

inline Context::Context(int flags) 
{ 
    SDL_Init(flags); 
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF);
    TTF_Init();
}

inline Context::~Context()
{
    SDL_Quit();
}

class Subsystem
{
  public:
    explicit Subsystem(int flags) : flags(flags) { SDL_InitSubSystem(flags); }
    ~Subsystem() { SDL_QuitSubSystem(flags); }

  public:
    bool wasInit() const { return SDL_WasInit(flags) & flags; }

  private:
    int flags;
};

namespace hints
{
    void clearHints();
    bool setHint(const std::string& name, const std::string& value);
    bool setHint(const std::string& name, const std::string& value, SDL_HintPriority priority);
    std::string getHint(const std::string& name);
}

namespace error
{
    std::string get();
    void clear();
    void set(const std::string& msg);
}

std::string getBasePath();
std::string getPrefPath(const std::string& org, const std::string& app);
void log(const std::string& msg);

}

#endif