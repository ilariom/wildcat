#ifndef _S2X_EVENTS_H
#define _S2X_EVENTS_H

#include <SDL2/SDL.h>
#include <unordered_map>
#include <functional>
#include <vector>
#include <algorithm>
#include <initializer_list>

namespace s2x
{

struct EventListener
{
    int tag;
    std::function<void(const SDL_Event&)> handler;
};

inline bool operator==(const EventListener& e, const EventListener& f)
{
    return e.tag == f.tag;
}

class EventManager
{
public:
    using EventType = Uint32;
    
public:
    inline bool poll();
    void push(SDL_Event& ev) { SDL_PushEvent(&ev); }
    void userInput() { while(poll()); }

    void addListener(EventType t, const EventListener& l) { this->listeners[t].push_back(l); }
    inline void addListener(std::initializer_list<EventType> events, const EventListener& l);
    inline void removeListener(EventType t, const EventListener& l);

private:
    SDL_Event cachedEvent;
    std::unordered_map<EventType, std::vector<EventListener>> listeners;
};

inline bool EventManager::poll()
{
    bool res = SDL_PollEvent(&cachedEvent);

    if(!res)
    {
        return false;
    }

    if(this->listeners.find(cachedEvent.type) == this->listeners.end())
    {
        return res;
    }

    auto& vec = this->listeners[cachedEvent.type];

    std::for_each(vec.begin(), vec.end(), [this] (const EventListener& el) {
        el.handler(cachedEvent);
    });
}

inline void EventManager::addListener(std::initializer_list<EventType> events, const EventListener& l)
{
    for(EventType t : events)
        addListener(t, l);
}

inline void EventManager::removeListener(EventType t, const EventListener& l)
{
    if(this->listeners.find(t) == this->listeners.end())
    {
        return;
    }

    auto& vec = this->listeners[t];
    auto it = std::find(vec.begin(), vec.end(), l);

    if(it != vec.end())
    {
        vec.erase(it);
    }
}

}

#endif