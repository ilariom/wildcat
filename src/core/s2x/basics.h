#ifndef _S2X_BASICS_H
#define _S2X_BASICS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <functional>
#include <memory>
#include <unordered_map>

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

class UserData
{
private:
    struct AbstractBag 
    {
        virtual ~AbstractBag() = default;
        virtual void* get() = 0;
    };

    template<typename T>
    class Bag : public AbstractBag
    {
    public:
        explicit Bag(T val) : val(val) { }
    public:
        void* get() override { return reinterpret_cast<void*>(&this->val); }
        
    private:
        T val;
    };

public:
    operator void*() const { return this->data->get(); }

public:
    template<typename T>
    T* get() { return reinterpret_cast<T*>(this->data->get()); }
    
    template<typename T>
    void set(const T& t) { this->data = std::make_shared<Bag<T>>(t); }

private:
    std::shared_ptr<AbstractBag> data;
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