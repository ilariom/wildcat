#ifndef _WKT_INPUT_MANAGER
#define _WKT_INPUT_MANAGER

#include "KeyboardProxy.h"
#include "MouseProxy.h"

namespace wkt {
namespace events
{

class InputManager
{
public:
    InputManager(const InputManager&) = delete;
    InputManager(InputManager&&) = delete;
    ~InputManager() = default;

    InputManager& operator=(const InputManager&) = delete;
    InputManager& operator=(InputManager&&) = delete;

    static inline InputManager& getInstance();

public:
    void setKeyboardProxy(KeyboardProxy* proxy) { this->keyboardProxy = proxy; }
    KeyboardProxy* getKeyboardProxy() const { return this->keyboardProxy; }

    void setMouseProxy(MouseProxy* proxy) { this->mouseProxy = proxy; }
    MouseProxy* getMouseProxy() const { return this->mouseProxy; }

private:
    InputManager() = default;
    KeyboardProxy* keyboardProxy = nullptr;
    MouseProxy* mouseProxy = nullptr;
};

inline InputManager& InputManager::getInstance()
{
    static InputManager im;
    return im;
}

}}

#endif