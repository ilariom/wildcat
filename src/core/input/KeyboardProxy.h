#ifndef _WKT_KEYBOARD_LISTENER_H
#define _WKT_KEYBOARD_LISTENER_H

#include "s2x/events.h"
#include <list>

namespace wkt {
namespace events
{

struct KeyboardEventType
{
    Uint32 eventType;
    SDL_Keycode code;
    Uint16 modifier;
    Uint8 state;
    bool repeat;
};

struct KeyboardEventTypeHasher
{
    size_t operator()(const KeyboardEventType& kt) const
    {
        return std::hash<int>()(kt.eventType) ^ std::hash<int>()(kt.code);
    }
};

class KeyboardProxy
{
public:
    void operator()(const SDL_Event& ev)
    {
        KeyboardEventType ket = {
            ev.type,
            ev.key.keysym.sym,
            ev.key.keysym.mod,
            ev.key.state,
            ev.key.repeat != 0
        };

        this->events.push_back(ket);
    }

    bool operator>>(KeyboardEventType& ket)
    {
        if(this->events.empty())
            return false;

        ket = std::move(this->events.front());
        this->events.pop_front();

        return true;
    }

private:
    std::list<KeyboardEventType> events;
};

}}

#endif