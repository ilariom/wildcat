#ifndef _WKT_INPUT_MANAGER
#define _WKT_INPUT_MANAGER

#include "KeyboardProxy.h"

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

private:
    InputManager() = default;
    KeyboardProxy* keyboardProxy = nullptr;
};

inline InputManager& InputManager::getInstance()
{
    static InputManager im;
    return im;
}

}}

#endif