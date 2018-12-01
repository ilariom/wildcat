#ifndef S2X_TYPES_H
#define S2X_TYPES_H

#include <SDL2/SDL.h>
#include <memory>

namespace s2x
{

using Point = SDL_Point;
using Rect = SDL_Rect;

struct Size
{
    int width, height;
};

inline SDL_Rect toSDLRect(const Point& origin, const Size& size)
{
    return { origin.x, origin.y, size.width, size.height };
}

inline Point pointFromSDLRect(const SDL_Rect& r)
{
    return { r.x, r.y };
}

inline Size sizeFromSDLRect(const SDL_Rect &r)
{
    return { r.w, r.h };
}

using Color = SDL_Color;

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

}

#endif