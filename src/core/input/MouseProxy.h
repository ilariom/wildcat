#ifndef _WKT_MOUSE_PROXY_H
#define _WKT_MOUSE_PROXY_H

#include "s2x/events.h"
#include <cstdint>
#include <vector>

namespace wkt {
namespace events
{

enum class ButtonEvent { UP, DOWN };
enum class ButtonState { PRESSED, RELEASED };
enum class ButtonType { LEFT, MIDDLE, RIGHT, X1, X2 };

struct MouseButtonEvent
{
    ButtonEvent event;
    ButtonType type;
    ButtonState state;
    uint8_t clicks;
    int32_t x;
    int32_t y;
};

inline ButtonType getButtonType(const SDL_MouseButtonEvent& ev)
{
    switch(ev.button)
    {
        case SDL_BUTTON_LEFT:       return ButtonType::LEFT;
        case SDL_BUTTON_MIDDLE:     return ButtonType::MIDDLE;
        case SDL_BUTTON_RIGHT:      return ButtonType::RIGHT;
        case SDL_BUTTON_X1:         return ButtonType::X1;
        case SDL_BUTTON_X2:         return ButtonType::X2;
    }
}

inline MouseButtonEvent mouseButtonEventFromSDL(const SDL_MouseButtonEvent& ev)
{
    return {
        ev.type == SDL_MOUSEBUTTONDOWN ? ButtonEvent::DOWN : ButtonEvent::UP,
        getButtonType(ev),
        ev.state == SDL_PRESSED ? ButtonState::PRESSED : ButtonState::RELEASED,
        (uint8_t) ev.clicks,
        (int32_t) ev.x,
        (int32_t) ev.y
    };
}

struct MouseMotionEvent
{
    int32_t x, y;
    int32_t dx, dy;
    std::vector<ButtonType> stateOfButtons;
};

inline std::vector<ButtonType> getButtonsDuringMotion(uint32_t state)
{
    std::vector<ButtonType> res;

    if(state & SDL_BUTTON_LMASK == SDL_BUTTON_LMASK)
        res.push_back(ButtonType::LEFT);

    if(state & SDL_BUTTON_MMASK == SDL_BUTTON_MMASK)
        res.push_back(ButtonType::MIDDLE);

    if(state & SDL_BUTTON_RMASK == SDL_BUTTON_RMASK)
        res.push_back(ButtonType::RIGHT);

    if(state & SDL_BUTTON_X1MASK == SDL_BUTTON_X1MASK)
        res.push_back(ButtonType::X1);

    if(state & SDL_BUTTON_X2MASK == SDL_BUTTON_X2MASK)
        res.push_back(ButtonType::X2);

    return res;
}

struct MouseWheelEvent
{
    int32_t dx, dy;
};

inline MouseWheelEvent& adjustMouseWheelEventForDirection(MouseWheelEvent&& mwe, uint32_t direction)
{
    if(direction == SDL_MOUSEWHEEL_FLIPPED)
    {
        mwe.dx *= -1;
        mwe.dy *= -1;
    }

    return mwe;
}

class MouseProxy
{
public:
    void operator()(const SDL_Event& ev)
    {
        switch(ev.type)
        {
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                this->buttonEvents.push_back(mouseButtonEventFromSDL(ev.button));
                break;

            case SDL_MOUSEMOTION:
                this->motionEvents.push_back({
                    ev.motion.x,
                    ev.motion.y,
                    ev.motion.xrel,
                    ev.motion.yrel,
                    getButtonsDuringMotion(ev.motion.state)
                });
                break;

            case SDL_MOUSEWHEEL:
                this->wheelEvents.push_back(adjustMouseWheelEventForDirection({
                    ev.wheel.x,
                    ev.wheel.y,
                }, ev.wheel.direction));
                break;
        }
    }

    std::vector<MouseButtonEvent>&& takeButtonEvents() { return std::move(this->buttonEvents); }
    std::vector<MouseMotionEvent>&& takeMotionEvents() { return std::move(this->motionEvents); }
    std::vector<MouseWheelEvent>&& takeWheelEvents() { return std::move(this->wheelEvents); }

private:
    std::vector<MouseButtonEvent> buttonEvents;
    std::vector<MouseMotionEvent> motionEvents;
    std::vector<MouseWheelEvent> wheelEvents;
};

}}

#endif